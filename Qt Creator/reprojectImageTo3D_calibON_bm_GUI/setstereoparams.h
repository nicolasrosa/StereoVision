#ifndef SETSTEREOPARAMS_H
#define SETSTEREOPARAMS_H

#include <QDialog>

namespace Ui {
class SetStereoParams;
}

class SetStereoParams : public QDialog
{
    Q_OBJECT

public:
    explicit SetStereoParams(QWidget *parent = 0);
    void setInitialStereoParamsUi(int preFilterSize,int preFilterCap,int SADWindowSize,int minDisparity,int numberOfDisparities,int textureThreshold,int uniquenessRatio, int speckleWindowSize, int speckleRange,int disp12MaxDiff);
    void getStereoParamsUi();
    ~SetStereoParams();

    bool isAlreadyShowing;

private slots:
    /* Sliders */
    void on_preFilterSize_slider_valueChanged(int value);
    void on_preFilterCap_slider_valueChanged(int value);
    void on_SADWindowSize_slider_valueChanged(int value);
    void on_minDisparity_slider_valueChanged(int value);
    void on_numberOfDisparities_slider_valueChanged(int value);
    void on_textureThreshold_slider_valueChanged(int value);
    void on_uniquenessRatio_slider_valueChanged(int value);
    void on_speckleWindowSize_slider_valueChanged(int value);
    void on_speckleRange_slider_valueChanged(int value);
    void on_disp12MaxDiff_slider_valueChanged(int value);

    /* SpinBoxes*/
    void on_preFilterSize_spinBox_valueChanged(int value);
    void on_preFilterCap_spinBox_valueChanged(int value);
    void on_SADWindowSize_spinBox_valueChanged(int value);
    void on_minDisparity_spinBox_valueChanged(int value);
    void on_numberOfDisparities_spinBox_valueChanged(int value);
    void on_textureThreshold_spinBox_valueChanged(int value);
    void on_uniquenessRatio_spinBox_valueChanged(int value);
    void on_speckleWindowSize_spinBox_valueChanged(int value);
    void on_speckleRange_spinBox_valueChanged(int value);
    void on_disp12MaxDiff_spinBox_valueChanged(int value);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::SetStereoParams *ui;
};

#endif // SETSTEREOPARAMS_H
