#include <QApplication>
#include <QMainWindow>

#include "screens/screenmanager.h"
#include "screens/mainmenuscreen.h"
#include "screens/optionsscreen.h"
#include "screens/creditsscreen.h"
#include "screens/gameboardscreen.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    window.resize(800, 600);
    window.setWindowTitle("Game_1");

    auto *screenManager = new ScreenManager(&window);
    screenManager->registerScreen(ScreenId::MainMenu, new MainMenuScreen());
    screenManager->registerScreen(ScreenId::Options, new OptionsScreen());
    screenManager->registerScreen(ScreenId::Credits, new CreditsScreen());
    screenManager->registerScreen(ScreenId::GameBoard, new GameBoardScreen());

    window.setCentralWidget(screenManager);
    screenManager->showScreen(ScreenId::MainMenu);

    window.show();

    return app.exec();
}
