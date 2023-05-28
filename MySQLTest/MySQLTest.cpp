﻿// MySQLTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <mysql.h>

using namespace std;

int create_tables(MYSQL* mysql);
int add_student(MYSQL* mysql);
int add_book(MYSQL* mysql);
int borrow_book(MYSQL* mysql);
int return_book(MYSQL* mysql);
int list_students(MYSQL* mysql);
int list_books(MYSQL* mysql);
int list_borrow(MYSQL* mysql);

int main()
{
    int res = 0;
    cout << "Connecting to MySQL...\n";
    MYSQL* mysql = mysql_init(NULL);
    if (mysql == NULL)
    {
        printf("mysql_init() error\n");
        return -1;
    }
    mysql = mysql_real_connect(mysql, "localhost", "root", "000112",
        "db_course", 3306, NULL, 0);
    if (mysql == NULL)
    {
        printf("Failed to connect to MySQL.\n");
        return -1;
    }

    printf("Successfully connected to MySQL\n");

    res=create_tables(mysql);
    if (res != 0) {
        std::cout<<"Failed to create tables.\n";
        return -1;
    }

    std::cout << "Successfully created tables.\n=====================================\n";

    // 循环
    char op;
    
    while (true) {
        std::cout << "Type h for help > ";
        std::cin>>op;
        switch (op) {
        case 'x': {
            mysql_close(mysql);
            cout<<"Bye"<<endl;
            return 0;
        }
		case 'h': {
			cout << "h - 查看帮助" << endl;
			cout << "x - 退出" << endl;
			cout << "a - 添加学生" << endl;
			cout << "b - 添加书籍" << endl;
			cout << "o - 借书" << endl;
			cout << "r - 还书" << endl;
			cout << "l - 查看所有学生" << endl;
			cout << "s - 查看所有书籍" << endl;
			cout << "i - 查看所有借阅记录" << endl;
			break;
		}
		case 'a': {
			res=add_student(mysql);
			break;
		}
		case 'b': {
			res=add_book(mysql);
			break;
		}
		case 'o': {
			res=borrow_book(mysql);
			break;
		}
		case 'r': {
			res=return_book(mysql);
			break;
		}
		case 'l': {
			res=list_students(mysql);
			break;
		}
		case 's': {
			res = list_books(mysql);
			break;
		}
		case 'i': {
			res = list_borrow(mysql);
			break;
		}
		default: {
			cout<<"Unknown command.\n";
			break;
		}
        }
    }

}

int create_tables(MYSQL* mysql) {
    int res = 0;

    res=mysql_query(
        mysql,
        "CREATE TABLE IF NOT EXISTS `Students`(`id` INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,`name` VARCHAR(20) NOT NULL)"
    );

    if (res != 0) {
        return res;
    }

    // 书籍表
    res=mysql_query(
		mysql,
		"CREATE TABLE IF NOT EXISTS `Books`(`id` INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,`name` VARCHAR(20) NOT NULL,`author` VARCHAR(20) NOT NULL,`stock` INT NOT NULL)"
	);

    if (res != 0) {
        return res;
    }

    // 借阅关系表
    res=mysql_query(
        mysql,
        "CREATE TABLE IF NOT EXISTS `Borrow`(`id` INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,`student_id` INT UNSIGNED NOT NULL,`book_id` INT UNSIGNED NOT NULL,`status` INT UNSIGNED NOT NULL,FOREIGN KEY (`student_id`) REFERENCES `Students`(`id`),FOREIGN KEY (`book_id`) REFERENCES `Books`(`id`))"
    );
    return res;
}

int add_student(MYSQL* mysql) {
	int res = 0;
	char name[20];
	std::cout << "Input student name > ";
	std::cin >> name;
	char sql[100];
	sprintf_s(sql, "INSERT INTO `Students`(`name`) VALUES('%s')", name);
	res = mysql_query(mysql, sql);
    if (res != 0) {
		std::cout << "Failed to add student.\n";
		return res;
	}
	std::cout << "Successfully added student.\n";
	return res;
}

int add_book(MYSQL* mysql) {
	int res = 0;
	char name[20];
	char author[20];
	int stock;
	std::cout << "Input book name > ";
	std::cin >> name;
	std::cout << "Input book author > ";
	std::cin >> author;
	std::cout << "Input book stock > ";
	std::cin >> stock;
	char sql[100];
	sprintf_s(sql, "INSERT INTO `Books`(`name`,`author`,`stock`) VALUES('%s','%s',%d)", name, author, stock);
	res = mysql_query(mysql, sql);
    if (res != 0) {
		std::cout << "Failed to add book.\n";
		return res;
	}
	std::cout << "Successfully added book.\n";
	return res;
}

int borrow_book(MYSQL* mysql) {
	int res = 0;
	int student_id;
	int book_id;
	std::cout << "Input student id > ";
	std::cin >> student_id;
	std::cout << "Input book id > ";
	std::cin >> book_id;
	char sql[100];
	sprintf_s(sql, "INSERT INTO `Borrow`(`student_id`,`book_id`,`status`) VALUES(%d,%d,0)", student_id, book_id);
	res = mysql_query(mysql, sql);
    if (res != 0) {
		std::cout << "Failed to borrow book.\n";
		return res;
	}
	std::cout << "Successfully borrowed book.\n";
	return res;
}

int return_book(MYSQL* mysql) {
	int res = 0;
	int student_id;
	int book_id;
	std::cout << "Input student id > ";
	std::cin >> student_id;
	std::cout << "Input book id > ";
	std::cin >> book_id;
	char sql[100];
	sprintf_s(sql, "UPDATE `Borrow` SET `status`=1 WHERE `student_id`=%d AND `book_id`=%d", student_id, book_id);
	res = mysql_query(mysql, sql);
    if (res != 0) {
		std::cout << "Failed to return book.\n";
		return res;
	}
	std::cout << "Successfully returned book.\n";
	return res;
}

int list_students(MYSQL* mysql) {
	int res = 0;
	MYSQL_RES* result;
	MYSQL_ROW row;
	res = mysql_query(mysql, "SELECT * FROM `Students`");
    if (res != 0) {
		std::cout << "Failed to list students.\n";
		return res;
	}
	result = mysql_store_result(mysql);
	if (result == NULL) {
		std::cout << "Failed to list students.\n";
		return res;
	}
	std::cout << "id\tname\n-------------------------------\n";
	while ((row = mysql_fetch_row(result))) {
		std::cout << row[0] << "\t" << row[1] << "\n";
	}
	mysql_free_result(result);
	return res;
}

int list_books(MYSQL* mysql) {
	int res = 0;
	MYSQL_RES* result;
	MYSQL_ROW row;
	res = mysql_query(mysql, "SELECT * FROM `Books`");
	if (res != 0) {
		std::cout << "Failed to list books.\n";
		return res;
	}
	result = mysql_store_result(mysql);
	if (result == NULL) {
		std::cout << "Failed to list books.\n";
		return res;
	}
	std::cout << "id\tname\tauthor\tstock\n-------------------------------\n";
	while ((row = mysql_fetch_row(result))) {
		std::cout << row[0] << "\t" << row[1] << "\t" << row[2] << "\t" << row[3] << "\n";
	}
	mysql_free_result(result);
	return res;
}

int list_borrow(MYSQL* mysql) {
	int res = 0;
	MYSQL_RES* result;
	MYSQL_ROW row;
	res = mysql_query(mysql, "SELECT `Borrow`.`id`,`Students`.`name`,`Books`.`name`,`Borrow`.`status` FROM `Borrow` INNER JOIN `Students` ON `Borrow`.`student_id`=`Students`.`id` INNER JOIN `Books` ON `Borrow`.`book_id`=`Books`.`id`");
	if (res != 0) {
		std::cout << "Failed to list borrow.\n";
		return res;
	}
	result = mysql_store_result(mysql);
	if (result == NULL) {
		std::cout << "Failed to list borrow.\n";
		return res;
	}
	std::cout << "id\tstudent\tbook\tstatus\n-------------------------------\n";
	while ((row = mysql_fetch_row(result))) {
		std::cout << row[0] << "\t" << row[1] << "\t" << row[2] << "\t" << (row[3][0] == '0' ? "borrowed" : "returned") << "\n";
	}
	mysql_free_result(result);
	return res;
}

