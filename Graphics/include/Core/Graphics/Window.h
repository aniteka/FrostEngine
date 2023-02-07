#pragma once
#include <core/transformable.h>
#include <core/movable.h>
#include <boost/noncopyable.hpp>

#include <optional>
#include <mutex>
#include <core/text.h>

#pragma comment(lib, "winmm.lib")
#include <windows.h>

namespace core
{
	struct window_info;

	class window final
		: public virtual transformable
		, public virtual movable
		, boost::noncopyable
	{
		using native_window_t = HWND__;
		struct native_deleter{void operator()(native_window_t* toDel){DestroyWindow(toDel);}};
		using native_window_ptr_t = std::unique_ptr <
			native_window_t,
			native_deleter> ;
	public:
		using info_t = window_info;

		explicit window(const info_t& createInfo);
		virtual ~window();
		window(window&&) noexcept = default;
		window& operator=(window&&) noexcept = default;

		[[nodiscard]] int get_width() const override;
		[[nodiscard]] int get_height() const override;
		void set_width(int width) override;
		void set_height(int height) override;
		void resize(int width, int height) override;
		[[nodiscard]] int get_x() const override;
		[[nodiscard]] int get_y() const override;
		void set_x(int x) override;
		void set_y(int y) override;
		void set_xy(int x, int y) override;
		void move(int dx, int dy) override;

		bool should_close() const;

		[[deprecated]]native_window_t* get_native();

	protected:
		native_window_ptr_t m_renderWindow = nullptr;
		mutable std::recursive_mutex m_windowMutex;

	private:
		void create_and_register_window_class(const ctext_t CLASS_NAME, const info_t& createInfo);
		void create_window(const ctext_t CLASS_NAME, const ctext_t WINDOW_TITLE, const info_t& createInfo);
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

	struct window_info
	{
		int width = 500;
		int height = 500;
		std::optional<const ctext_t> title;
	};

}
