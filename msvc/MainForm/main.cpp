#include <MainForm.hpp>
#define  WIN32_LEAN_AND_MEAN
#include <Windows.h>

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    MainForm fm;
    fm.events_processing();

    fm.show();

    nana::exec();

    return 0;
}
