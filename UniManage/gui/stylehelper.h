#ifndef STYLEHELPER_H
#define STYLEHELPER_H

#include <QString>

class StyleHelper {
public:
    static QString getMainStyle() {
        return R"(
            QWidget {
                background-color: #f8fafc;
                font-family: 'Segoe UI', Roboto, Helvetica, Arial, sans-serif;
                font-size: 13px;
                color: #334155;
            }

            QMainWindow, QDialog {
                background-color: #f1f5f9;
            }

            QTabWidget::pane {
                border: 1px solid #e2e8f0;
                background: white;
                border-radius: 8px;
                top: -1px;
            }

            QTabBar::tab {
                background: #f1f5f9;
                border: 1px solid #e2e8f0;
                padding: 8px 16px;
                border-top-left-radius: 8px;
                border-top-right-radius: 8px;
                margin-right: 2px;
                color: #64748b;
            }

            QTabBar::tab:selected {
                background: white;
                border-bottom-color: white;
                color: #2563eb;
                font-weight: bold;
            }

            QPushButton {
                background-color: #2563eb;
                color: white;
                border-radius: 6px;
                padding: 8px 16px;
                font-weight: 600;
                border: none;
            }

            QPushButton:hover {
                background-color: #1d4ed8;
            }

            QPushButton:pressed {
                background-color: #1e40af;
            }

            QPushButton#secondaryBtn {
                background-color: #64748b;
            }

            QPushButton#secondaryBtn:hover {
                background-color: #475569;
            }

            QPushButton#dangerBtn {
                background-color: #ef4444;
            }

            QPushButton#dangerBtn:hover {
                background-color: #dc2626;
            }

            QLineEdit, QSpinBox, QComboBox, QDateTimeEdit, QTextEdit {
                background-color: white;
                border: 1px solid #cbd5e1;
                border-radius: 6px;
                padding: 6px 12px;
                selection-background-color: #bfdbfe;
            }

            QLineEdit:focus, QSpinBox:focus, QComboBox:focus {
                border: 2px solid #3b82f6;
            }

            QTableWidget {
                background-color: white;
                alternate-background-color: #f8fafc;
                gridline-color: #f1f5f9;
                border: 1px solid #e2e8f0;
                border-radius: 8px;
                selection-background-color: #eff6ff;
                selection-color: #1e40af;
            }

            QHeaderView::section {
                background-color: #f8fafc;
                padding: 10px;
                border: none;
                border-bottom: 2px solid #e2e8f0;
                font-weight: bold;
                color: #475569;
            }

            QLabel#titleLabel {
                font-size: 24px;
                font-weight: 800;
                color: #1e293b;
                margin-bottom: 20px;
            }

            QLabel#subtitleLabel {
                font-size: 16px;
                font-weight: 600;
                color: #475569;
            }

            QGroupBox {
                font-weight: bold;
                border: 1px solid #e2e8f0;
                border-radius: 8px;
                margin-top: 20px;
                padding-top: 20px;
            }

            QGroupBox::title {
                subcontrol-origin: margin;
                left: 10px;
                padding: 0 5px;
            }
        )";
    }
};

#endif // STYLEHELPER_H
