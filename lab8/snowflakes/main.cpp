#include "DisplayWrapper.h"
#include "GCWrapper.h"
#include "Timer.h"
#include <X11/Xlib.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <random>
#include <unistd.h>
#include <vector>

constexpr int MAX_SNOWFLAKES = 2000;
constexpr int SNOWFLAKES_SPEED = 20;
constexpr int SNOWFLAKES_SIZE = 2;
constexpr int SLEEP_DURATION = 50000;
constexpr int ARC_MIN_ANGLE = 0;
constexpr int ARC_MAX_ANGLE = 64 * 360;

int GetRandomInt(const int min, const int max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

	return static_cast<int>(dist(rng));
}

struct Snowflake
{
	double x;
	double y;
	double speed;
	int size;
};

using Snowflakes = std::vector<Snowflake>;

void InitSnowflakes(Snowflakes& snowflakes, const int width, int const height)
{
	for (auto& [x, y, speed, size] : snowflakes)
	{
		const auto randomInt = GetRandomInt(1, 5);
		x = GetRandomInt(0, width);
		y = GetRandomInt(0, height);
		speed = randomInt * SNOWFLAKES_SPEED;
		size = randomInt * SNOWFLAKES_SIZE;
	}
}

void UpdateSnowflakes(Snowflakes& snowflakes, const int width, int const height, const double deltaTime)
{
	// добавить движения по горизонтали
	for (auto& flake : snowflakes)
	{
		flake.y += flake.speed * deltaTime;
		if (flake.y > height)
		{
			flake.y = 0;
			flake.x = GetRandomInt(0, width);
		}
	}
}

void DrawSnowflakes(Display* display, const Window& window, GC gc, Snowflakes& snowflakes, const int width, const int height)
{
	for (const auto& flake : snowflakes)
	{
		if (flake.x < width && flake.y < height)
		{
			XFillArc(display, window, gc, static_cast<int>(flake.x), static_cast<int>(flake.y), flake.size, flake.size, 0, ARC_MAX_ANGLE);
		}
	}
}

Window CreateWindow(Display* display)
{
	const int screen = DefaultScreen(display);
	const auto displayWidth = DisplayWidth(display, 0);
	const auto displayHeight = DisplayHeight(display, 0);

	return XCreateSimpleWindow(display, RootWindow(display, screen),
		0, 0, displayWidth, displayHeight, 0,
		BlackPixel(display, screen), BlackPixel(display, screen));
}

void InitGC(Display* display, GC gc)
{
	const int screen = DefaultScreen(display);
	XSetForeground(display, gc, WhitePixel(display, screen));
}

int main()
{
	const auto displayKeeper = DisplayWrapper();
	const auto display = displayKeeper.Get();

	const auto window = CreateWindow(display);

	XSelectInput(display, window, ExposureMask | KeyPressMask | StructureNotifyMask);
	XMapWindow(display, window);

	const GCWrapper gcKeeper(displayKeeper, window);
	const auto gc = gcKeeper.Get();

	const auto displayWidth = DisplayWidth(display, 0);
	const auto displayHeight = DisplayHeight(display, 0);

	Snowflakes snowflakes(MAX_SNOWFLAKES);
	InitSnowflakes(snowflakes, displayWidth, displayHeight);
	InitGC(display, gc);
	Timer timer;

	XEvent event;
	auto currentWidth = displayWidth;
	auto currentHeight = displayHeight;
	while (true)
	{
		if (XPending(display))
		{
			XNextEvent(display, &event);
			if (event.type == KeyPress || event.type == DestroyNotify)
			{
				break;
			}
			if (event.type == ConfigureNotify)
			{
				XWindowAttributes xwAttr;
				XGetWindowAttributes(display, window, &xwAttr);
				currentWidth = xwAttr.width;
				currentHeight = xwAttr.height;
			}
		}

		XClearWindow(display, window);
		DrawSnowflakes(display, window, gc, snowflakes, currentWidth, currentHeight);
		XFlush(display);

		const auto deltaTime = timer.Restart();
		UpdateSnowflakes(snowflakes, displayWidth, displayHeight, deltaTime);
		// Сделать без sleep
		usleep(SLEEP_DURATION);
	}

	return EXIT_SUCCESS;
}