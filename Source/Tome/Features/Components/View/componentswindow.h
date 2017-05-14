#ifndef COMPONENTSWINDOW_H
#define COMPONENTSWINDOW_H

#include <QMainWindow>

#include "../Model/component.h"

class ComponentWindow;

namespace Ui {
    class ComponentsWindow;
}

namespace Tome
{
    class ComponentsController;
    class FieldDefinitionsController;
    class UndoController;
}

/**
 * @brief Window for adding and removing components.
 */
class ComponentsWindow : public QMainWindow
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs a new window for adding and removing components.
         * @param componentsController Controller for adding and removing components.
         * @param fieldDefinitionsController Controller for adding, updating and removing field definitions.
         * @param undoController Controller for performing undo-able commands.
         * @param parent Optional owner widget.
         */
        explicit ComponentsWindow(Tome::ComponentsController& componentsController,
                                  Tome::FieldDefinitionsController& fieldDefinitionsController,
                                  Tome::UndoController& undoController,
                                  QWidget *parent = 0);
        ~ComponentsWindow();

    private slots:
        void on_actionNew_Component_triggered();
        void on_actionDelete_Component_triggered();

        void onComponentAdded(const Tome::Component& component);
        void onComponentRemoved(const Tome::Component& component);

    private:
        Ui::ComponentsWindow *ui;

        Tome::ComponentsController& componentsController;
        Tome::FieldDefinitionsController& fieldDefinitionsController;
        Tome::UndoController& undoController;

        ComponentWindow* componentWindow;
};

#endif // COMPONENTSWINDOW_H
