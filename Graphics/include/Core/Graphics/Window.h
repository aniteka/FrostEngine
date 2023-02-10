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
		struct native_deleter{void operator()(native_window_t* to_del){DestroyWindow(to_del);}};
		using native_window_ptr_t = std::unique_ptr <
			native_window_t,
			native_deleter> ;
	public:
		using info_t = window_info;
		enum class window_close_state 
		{
			closed, should_close, unclosed
		};

		explicit window(const info_t& create_info);
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

		void close();
		bool should_close();
		window_close_state close_state() const;

		[[deprecated, nodiscard]] native_window_t* get_native() const;

	protected:
		native_window_ptr_t m_render_window = nullptr;
		mutable std::recursive_mutex m_window_mutex;
		window_close_state m_should_close = window_close_state::unclosed;

	private:
		void create_and_register_window_class(ctext_t CLASS_NAME, const info_t& create_info);
		void create_window(ctext_t CLASS_NAME, ctext_t WINDOW_TITLE, const info_t& create_info);

#ifdef WIN32
		LRESULT window_proc_callback(HWND window_handler, UINT msg, WPARAM wparam, LPARAM lparam);
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
	};

	struct window_info
	{
		int width = 500;
		int height = 500;
		std::optional<ctext_t> title;
	};

}
