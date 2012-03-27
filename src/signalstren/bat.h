#include <QWidget>

/**
  * The main window class.
  * Kept here because Qt Mobility needs Qt facilities to operate.
  *
  * TODO: use qt mobility...
  */
class bat: public QWidget
{
    Q_OBJECT
public:
    bat(QWidget * = NULL) {
        main_sequence();
    }
private:
    /**
      * Main signalstren operation sequence: scan-measure-evaluate-modify
      */
    void main_sequence();
};
