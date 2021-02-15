
# PERSONAL PROJECT PLANNER
#### Video Demo:  https://youtu.be/qYpWkX_kBic
#### Description:
A web application that allows the users to have different projects and plan them through tasks.
  
![alt text](https://github.com/me50/montsyglz/blob/cs50/problems/2021/x/project/static/photos/start_page.png)

## Table of contents
* [General info](#general-info)
* [HTML Files](#html-files)
* [Flask/ Python Files](#flask-python-files)
* [Database](#database)

## General info
This project is a planner for personal projects someone is working on. The user have multiple projects, and multiple tasks. The tasks can also be filtered so the tasks can be viewed in a way that is convenient to the user.

For this project, parts of an assignment from the CS50 Introduction to Computer Science were used as a starting point for the creation of this webapp. The assignment used as a starting point was one called finance from the web programming tracks.

## HTML Files
Multiple html templates were created for this webapp, which are explained in the following paragraphs.

#### apology.html
This html is used to notify the user of something they might have missed to provide, or of other errors.

#### edit.html
This html is used when the user decided to edit some task, so it shows the input forms filled in with the specified values of the task, which can be updated.

![alt text](https://github.com/me50/montsyglz/blob/cs50/problems/2021/x/project/static/photos/edit_html.png)

#### home.html
This shows up right a user registers or logs into their account. They are greeted with a form they can fill in to add a project they're working on, as well as a list of the projects with the number of tasks of each project.

![alt text](https://github.com/me50/montsyglz/blob/cs50/problems/2021/x/project/static/photos/home_page_user.png)

#### layout.html
This file is the template for the rest of the html files.

#### login1.html
This html file shows the input forms so the user can login into their account.

![alt text](https://github.com/me50/montsyglz/blob/cs50/problems/2021/x/project/static/photos/login_page.png)

#### register1.html
Here the user is greeted with the input forms to make an account.

![alt text](https://github.com/me50/montsyglz/blob/cs50/problems/2021/x/project/static/photos/register_page.png)

#### start.html
This file shows up when someone accesses the webapp for the first time.

![alt text](https://github.com/me50/montsyglz/blob/cs50/problems/2021/x/project/static/photos/start_page.png)

#### tasks.html
This html file shows a form to fill out when the user wishes to add a task, as well as a table with the tasks of the user. This page also includes a select component that allows users to filter their tasks, so they can have different views of them. They can also mark their tasks as done, or not, as well as click buttons in order to edit or erase a task.

![alt text](https://github.com/me50/montsyglz/blob/cs50/problems/2021/x/project/static/photos/tasks_page_user.png)

## Flask/ Python Files

#### application.py
This is the flask document, which includes the different functions that help the webapp interact with the user through post and get methods.

#### helpers.py
This document includes two functions provided by CS50 Introduction to Computer Science. One is for handling errors, and another is for the login to be required.

## Database
For the database SQLite version: 3.31.1 was used. The database project.db includes three tables that include the users, the projects of the users, as well as another table for the tasks.
