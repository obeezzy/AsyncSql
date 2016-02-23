# Async SQL

The asynchronous version of **QSqlTableModel**.

I'm sure you love **QSqlTableModel** as much as I do. The only problem with it is that it's synchronous (which means that all queries run on the UI thread). This makes them unusable for large data sets or complex database queries or slow database access over a network (because it would cause your UI to freeze occasionally).

The solution? Combine the ease of using **QSqlTableModel** with a powerful single-thread backend. It's basically **QSqlTableModel** on steroids!

I conceived the concept of this project after reading [this article](http://www.linuxjournal.com/article/9602?page=0,0) about a year ago. Thanks Dave Berton!

**NOTE**: I will make this article sound a lot more professional later. Let me have my fun for now!

## Features
- Works for the **SQLite** and **MySQL** databases (only)
- Allows for limit setting
- Can be used to display tables that requires foreign key (it's still pretty difficult for now, but I've done it before)
- Every other feature that the **QSqlTableModel** has

## How to use
The project itself is an example of how to use this library. However, here's a preview:

    // In header file
    AsyncSql::AsyncSqlTableModel *model;
    ...
    
    // In source file
    using namespace AsyncSql;
    ...
    // Set database connection options using the DatabaseConnection class
    DatabaseConnection::setDefaultDriver(DatabaseConnection::SQLite);
    ...
    model = new AsyncSqlTableModel(this);
    model->setTable("sales");
    
    // Using C++11
    connect(model, &AsyncSqlTableModel::selected, [this](bool successful)
    {
        // Display the first record if successful.
        if(successful)
            qDebug() << "Hello AsyncSql!" << model->record(0);
        else
            qDebug() << "Error discovered: " << model->lastError().text();
    });
    
You can view the table with a **QTableView**, since **AsyncTableModel** inherits **QAbstractTableModel**.
    
    // In header file...
    QTableView *view;
    
    // In source file...
    view = new QTableView(this);
    view->setModel(model);

## Dependencies
- [Qt 5] framework

## Todo
* Database transaction handling
* More examples (especially one that uses **QML**)
* Other databases
* Allow for custom operations on the database thread (work in progress)
 
Lastly, as I always say:

**Please report all bugs. 
Also, I am FAR FROM perfect. If you see anything that can be done better, please notify me.**

License
----

MIT

[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)

   [Qt 5]: <https://qt.gitorious.org/qt/qt5>