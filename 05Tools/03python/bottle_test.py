#! /usr/bin/env python
# coding=utf-8
__author__ = 'peimin@outlook.com'

import urllib2
import hashlib
import ConfigParser
import os
import time
import logging
import sys
import MySQLdb
import urllib
import daemon
import hmac
import json

from bottle import route, run, template, get, post, request

APP_URL     = None
APP_ID      = None
APP_KEY     = None
LOG         = None
con         = None
curs        = None

# --------------------------------------------------- 配置文件 --------------------------------------------
config_path = "./config.cfg"
config_path = os.path.abspath(os.path.expanduser(config_path))
if not os.path.exists(config_path):
    print "Please Configure Your send_config.cfg "
    sys.exit(0)
config = ConfigParser.RawConfigParser()
config.read(config_path)

APP_KEY     = config.get('pay', 'app_key')
APP_ID      = config.getint('pay', 'app_id')
db_host     = config.get('database', 'host')
db_name     = config.get('database', 'db_name')
db_user     = config.get('database', 'user')
db_passwd   = config.get('database', 'passwd')

# ------------------------------------------------- 日志处理 --------------------------------------------
def log_init(log_file, log_debug_info, quiet=False):
    logger = logging.getLogger()
    if log_debug_info:
        logger.setLevel(logging.DEBUG)
    else:
        logger.setLevel(logging.INFO)

    hdlr = logging.FileHandler(log_file)
    formatter = logging.Formatter("%(process)s %(asctime)s %(pathname)s:%(lineno)d %(levelname)s %(message)s")
    hdlr.setFormatter(formatter)
    logger.addHandler(hdlr)
    if not quiet:
        hdlr = logging.StreamHandler()
        hdlr.setFormatter(formatter)
        logger.addHandler(hdlr)
    return logger

log_file        = os.path.abspath(os.path.expanduser(config.get('log', 'path')))
log_debug_info  = config.getboolean('log', 'debug_info')
log_quiet       = config.getboolean('log', 'quiet')		
LOG = log_init(log_file, log_debug_info, log_quiet)	# 初始化日志

# --------------------------------------------------- POST ---------------------------------------------

@post('/test') 
def deal_pay():	
    print "POST Header : \n %s" % dict(request.headers) #for debug header

    req_data = request.json

    test    = str(req_data['test'])
    test1   = req_data['test1']  # return json

    print "req_data : %s" % req_data 
    print "test     : %s" % test
    print "test1    : %s" % test1

    try:
        con = MySQLdb.connect(host=db_host, db=db_name, user=db_user, passwd=db_passwd, charset="utf8")
        curs = con.cursor()
        curs.execute("set names utf8")
    except MySQLdb.Error, e:	
        return  DEAL_FAILURE

    curs.execute("set wait_timeout=28800")
    curs.execute("set interactive_timeout=28800")

    
    # cpId + amount=...+callbackInfo=...+failedDesc=...+gameId=...+orderId=...+orderStatus=...+payWay=...+serverId=...+ucid=...+apiKey
     
    # md5验证数据的完整性
    sign_buf = "%d%s%s" % (...)     
    calc_sign = hashlib.md5(sign_buf).hexdigest().lower() 
    
    print "calc_sign : %s" % calc_sign
    print "sign      : %s" % sign

    if sign != calc_sign:
        sql = " "
        curs.execute(sql)
        con.commit()	
        return ...

    return ...

run(host='127.0.0.1', port=8080,  debug=True, quiet=False)

# ------------------------------------------------ GET -------------------------------------------------

# htttp........ ? xxx=xx&xxx=xxx&................................
@route('/test')
def deal_pay(): 
    test = request.GET.get('test', None)

# ------------------------------------------------------------------------------------------------------
  