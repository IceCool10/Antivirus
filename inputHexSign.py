import MySQLdb
import sys
import os

def insertHexSign(category,signature):
    # Open database connection
    db = MySQLdb.connect("localhost","maco","Pass/123","Antivirus" )

    # prepare a cursor object using cursor() method
    cursor = db.cursor()

    # execute SQL query using execute() method.
    cursor.execute("INSERT INTO hexSigns (category,signature) values (%s,%s)",(category,signature))

    # Fetch a single row using fetchone() method.
    db.commit()

    # disconnect from server
    db.close()

"""
if len(sys.argv) <= 1:
    print "usage " + os.path.basename(sys.argv[0]) + " hex_signature virus_category"
else:
    insertHexSign(sys.argv[2],sys.argv[1])
    """
category    =   raw_input("""Enter category: """)
signature   =   "\n".join(iter(raw_input,""))
signature.replace('\n','').replace('\r','')
insertHexSign(category,signature)
