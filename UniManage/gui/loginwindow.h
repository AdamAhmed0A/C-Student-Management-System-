#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void onUserTypeChanged(int index);
    void onLoginClicked();
    void onTestConnectionClicked();

private:
    void setupUI();
    bool tryLogin(const QString& username, const QString& password, const QString& role);

    QComboBox* m_userTypeCombo;
    QLabel* m_usernameLabel;
    QLineEdit* m_usernameEdit;
    QLabel* m_passwordLabel;
    QLineEdit* m_passwordEdit;
    QPushButton* m_loginButton;
    QPushButton* m_testConnButton;

    int m_currentUserId;
};

#endif // LOGINWINDOW_H
