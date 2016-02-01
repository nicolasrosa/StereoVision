#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
/**
 * @brief The MainWindow class is the base class from QT Creator that creates the main QWidget called MainWindow. All
 * of the program is going ot be shown into this window
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief The Default constructor created by the Qt
     * @param parent is the argument for the base class. For us, is the QWidget class
     */
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    /**
     * @brief on_pushButton_clicked is the slot created by the Qt. When the button is pressed, this function will be called
     */
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
