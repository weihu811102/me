#!/usr/bin/python
# -*- coding: utf-8 -*-

import urllib2
import hashlib
import ConfigParser
import os
import time
import logging
import sys
import MySQLdb
import urllib
import json

LOG  = None
con  = None
curs = None

config_path = "./config.cfg"
config_path = os.path.abspath(os.path.expanduser(config_path))
if not os.path.exists(config_path):
    print "Please Configure Your send_config.cfg "
    sys.exit(0)
config = ConfigParser.RawConfigParser()
config.read(config_path)

db_host     = config.get('database', 'host')
db_name     = config.get('database', 'db_name')
db_user     = config.get('database', 'user')
db_passwd   = config.get('database', 'passwd')

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
is_daemon       = config.getboolean('pay', 'daemon')

def one_request():    
    try:
        con = MySQLdb.connect(host=db_host, db=db_name, user=db_user, passwd=db_passwd, charset="utf8")
        curs = con.cursor()
        curs.execute("set names utf8")
    except MySQLdb.Error, e:
        LOG.error("Error %d: %s" % (e.args[0], e.args[1]))

        return
    curs.execute("set wait_timeout=28800")
    curs.execute("set interactive_timeout=28800")

    msg_data = .....
    clientsecret = hashlib.md5(msg_data).hexdigest().lower()

    url_dict = "xxxx=" + str(xxxx) + "&xxxx=" + str(xxxxx)

    # 格式化成http请求的格式
    requset_url = str(url) + "?" + str(url_dict) 
    LOG.info(requset_url)
    try:
        rece_content = urllib2.urlopen(requset_url).readlines()
        if rece_content:
            LOG.info(rece_content)

            ddata= json.loads(tmp_str)
            back_result = int(ddata['Result'])

            print back_result

    except urllib2.URLError, e:
        try:
            import ipdb
            ipdb.set_trace()
        except ImportError:
            print "Not Import ipdb"
        LOG.error(e)
    
curs.close()
con.close()
time.sleep(SLEEP_TIME)

if __name__ == "__main__":
    if is_daemon:
            LOG = log_init(log_file, log_debug_info, log_quiet)
            while True:
                try:
                    one_request()                
                except KeyboardInterrupt:
                    break
    else:
        LOG = log_init(log_file, log_debug_info, log_quiet)
        one_request() 