#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void onLoginClicked();
    void onTestConnectionClicked();

private:
    void setupUI();
    bool tryLogin(const QString& idNumber, const QString& password);

    QLabel* m_usernameLabel;
    QLineEdit* m_usernameEdit;
    QLabel* m_passwordLabel;
    QLineEdit* m_passwordEdit;
    QPushButton* m_loginButton;
    QPushButton* m_testConnButton;

    int m_currentUserId;
    QString m_userRole;
};

#endif // LOGINWINDOW_H
