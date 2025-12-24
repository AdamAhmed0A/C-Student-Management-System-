# FIX: Database Connection Error

## 🔴 Error: "Failed to initialize database"

This error means the application cannot connect to your MySQL database.

## 🔍 Common Causes:

1. **Wrong MySQL password** (most common)
2. MySQL service not running
3. Wrong host/port
4. Database doesn't exist
5. User doesn't have permissions

## ✅ Solution (Choose One):

### Option 1: Quick Fix - Set Your MySQL Password

**Run this command in PowerShell (as Administrator):**

```powershell
[System.Environment]::SetEnvironmentVariable('DB_PASSWORD', 'YOUR_ACTUAL_PASSWORD', 'User')
```

Replace `YOUR_ACTUAL_PASSWORD` with your actual MySQL root password.

Then **restart** the application.

### Option 2: Use Configuration Helper

**Double-click:**

```
configure_db.bat
```

It will ask you for:

- MySQL Host (default: 127.0.0.1)
- MySQL Port (default: 3306)
- MySQL Username (default: root)
- MySQL Password (YOUR PASSWORD HERE!)
- Database Name (default: university)

Then **restart** the application.

### Option 3: Manual Environment Variables

1. Open **System Properties** -> **Environment Variables**
2. Under **User variables**, click **New**
3. Add these variables:
   - Name: `DB_HOST`, Value: `127.0.0.1`
   - Name: `DB_PORT`, Value: `3306`
   - Name: `DB_USER`, Value: `root`
   - Name: `DB_PASSWORD`, Value: `your_mysql_password`
   - Name: `DB_NAME`, Value: `university`
4. Click OK
5. **Restart** the application

## 🔧 Verify MySQL is Running

**Check if MySQL is running:**

```batch
sc query MySQL80
```

**If not running, start it:**

```batch
net start MySQL80
```

(Replace `MySQL80` with your MySQL service name)

## 🧪 Test MySQL Connection Manually

**Try connecting with mysql command:**

```batch
mysql -u root -p
```

Enter your password. If this works, use that password in the configuration!

## 📊 Check the Console Output

After rebuilding and running the app, check the console for:

```
=== ATTEMPTING DATABASE CONNECTION ===
Host: 127.0.0.1
Port: 3306
User: root
Database: university
Password: (set)
```

If you see:

```
=== CANNOT CONNECT TO MYSQL SERVER ===
Error: Access denied for user 'root'@'localhost'
```

This means **wrong password**! Use one of the options above to set the correct password.

If you see:

```
Error: Can't connect to MySQL server on '127.0.0.1'
```

This means **MySQL is not running**! Start the MySQL service.

## 🎯 Quick Checklist

- [ ] MySQL service is running
- [ ] You know your MySQL root password
- [ ] Database password is set via environment variable
- [ ] Application has been restarted after setting variables
- [ ] Database 'university' exists (or app will create it)

## 🚀 After Fixing Connection

Once connected, the app will:

1. ✅ Connect to MySQL
2. ✅ Create 'university' database if it doesn't exist
3. ✅ Create all tables automatically
4. ✅ Show the login screen

Then you can login with:

- Username: `admin`
- Password: `admin123`

## 💡 Pro Tip

The easiest way is to just set the DB_PASSWORD environment variable:

```powershell
# In PowerShell (as Admin):
[System.Environment]::SetEnvironmentVariable('DB_PASSWORD', 'your_password', 'User')
```

Then restart the app. That's it!

## 📝 Still Not Working?

1. Rebuild the application (it now has better error messages)
2. Run it
3. Copy the ENTIRE console output
4. Share it with me

The new error messages will tell us exactly what's wrong!
