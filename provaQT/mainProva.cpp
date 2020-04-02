//
// Created by antonio on 23/03/20.
//


int main(int argc, char** argv){
    QApplication app(argc, argv);
    QMainWindow* mainWindow;
    Ui::MyTextEditor textProva{};

    textProva.setupUi(mainWindow);
    mainWindow->show();
    return app.exec();

}
