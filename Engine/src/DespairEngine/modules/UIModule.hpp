#ifndef ENGINE_UI_MODULE_HPP
#define ENGINE_UI_MODULE_HPP

struct GLFWwindow;

namespace engine {

    class UIModule
    {
    public:
        static void onWindowCreate(GLFWwindow* pWindow);
        static void onWindowClose();
        static void onUIDrawBegin();
        static void onUIDrawEnd();
    };

}

#endif // ENGINE_UI_MODULE_HPP