#!/usr/bin/python
# -*- coding: utf-8 -*-

import MySQLdb
import sys

con = None
con = MySQLdb.connect(host="localhost", db="game", user="root", passwd="123456", charset="utf8")
curs = con.cursor();
curs.execute("set names utf8");

if __name__ == '__main__':
    for account_id in xrange(begin_player_num, end_player_num):
        icon_id = account_id % 2
        sex = icon_id
        sql = ''
        curs.execute(sql)
        curs.execute("SELECT LAST_INSERT_ID()")
        player_id = curs.fetchone()[0]
        for i in xrange(0, 5):
            sql = ""
            curs.execute(sql)
            rows = curs.fetchone()
            skill_id = 0
            group1_active = 0
            group2_active = 0
            group3_active = 0
            if i:
                is_active = 0
                array_num = "NULL"
            else:
                is_active = 1
                array_num = 1
            level_up_progress = 0

    con.commit();
        

    
