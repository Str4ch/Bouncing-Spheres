#include "rtweekend.hpp"
#include <windows.h>
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "plane.hpp"

const int WIDTH = 400;
const int HEIGHT = 225;
COLORREF pixels[WIDTH * HEIGHT];
HBITMAP hBitmap = nullptr;
HDC hMemDC = nullptr;

color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        if (rec.t > 1) {
            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5 * (unit_direction.y() + 1.0);
            return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
        }
        return color(1 - rec.t, 1 - rec.t, 1 - rec.t);// 0.5 * (rec.normal + color(1, 1, 1)) / rec.t;
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}


auto aspect_ratio = 16.0 / 9.0;
hittable_list world;

void add_items() {
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    //world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
    world.add(make_shared<plane>(0, 0, 1, -0.5));
}

// Camera

auto focal_length = 1.0;
auto viewport_height = 2.0;
auto viewport_width = viewport_height * (double(WIDTH) / HEIGHT);
auto camera_center = point3(0, 0, 0);
// Calculate the vectors across the horizontal and down the vertical viewport edges.
auto viewport_u = vec3(viewport_width, 0, 0);
auto viewport_v = vec3(0, -viewport_height, 0);

// Calculate the horizontal and vertical delta vectors from pixel to pixel.
auto pixel_delta_u = viewport_u / WIDTH;
auto pixel_delta_v = viewport_v / HEIGHT;

// Calculate the location of the upper left pixel.
auto viewport_upper_left = camera_center
- vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


void render_frame() {
    // Render

    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r, world);
            color fin_pixel = write_color(pixel_color);
            pixels[j * WIDTH + i] = RGB(fin_pixel.x(), fin_pixel.y(), fin_pixel.z());
        }
    }

}
double angle = 0;
void update_frame() {
    angle += 0.01;
    world.objects.pop_back();
    world.add(make_shared<sphere>(point3(0, sin(angle), -1), 0.5));
    if (angle > 7) {
        angle = 0;
    }
}
/*void FillPixelArray() {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            pixels[y * WIDTH + x] = RGB(rand() % 256, rand() % 256, rand() % 256); // Случайные цвета
        }
    }
}*/

void CreateBitmapFromArray(HDC hdc) {
    if (hBitmap) {
        DeleteObject(hBitmap);
    }

    hBitmap = CreateBitmap(WIDTH, HEIGHT, 1, 32, pixels);

    if (!hMemDC) {
        hMemDC = CreateCompatibleDC(hdc);
    }

    SelectObject(hMemDC, hBitmap);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        update_frame();
        CreateBitmapFromArray(hdc);

        BitBlt(hdc, 0, 0, WIDTH, HEIGHT, hMemDC, 0, 0, SRCCOPY);

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_TIMER: {
        //FillPixelArray();

        render_frame();

        InvalidateRect(hwnd, NULL, TRUE);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"PixelArrayWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    // Создаем окно
    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Pixel Array Window", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL
    );

    if (!hwnd) {
        return 0;
    }

    //FillPixelArray();
    add_items();
    render_frame();

    SetTimer(hwnd, 1, 1, NULL);


    ShowWindow(hwnd, nCmdShow);


    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (hMemDC) {
        DeleteDC(hMemDC);
    }
    if (hBitmap) {
        DeleteObject(hBitmap);
    }

    return 0;
}
