import Tkinter
import tkMessageBox
from Tkinter import Tk
from tkFileDialog import askopenfilename, asksaveasfile, askdirectory
from Tkinter import *
import subprocess
import os
import re


top     =   Tkinter.Tk()
def scanFile():
    filename    =   askopenfilename()
    if (filename != "" ):
        try:
            output      =   subprocess.Popen(["./antiv","scan",filename], stdin=subprocess.PIPE,stderr=subprocess.PIPE,stdout=subprocess.PIPE).communicate()[0]
            r= re.compile("\033\[[0-9;]+m")
            output = r.sub("",output)
            #tkMessageBox.showinfo("Scan Report", output)
            box =   Tk()
            box.geometry("350x200")
            box.title("Sign File Report")
            s = Tkinter.Scrollbar(box)
            T = Tkinter.Text(box)

            T.focus_set()
            s.pack(side=Tkinter.RIGHT, fill=Tkinter.Y)
            T.pack(side=Tkinter.LEFT, fill=Tkinter.Y)
            s.config(command=T.yview)
            T.config(yscrollcommand=s.set)
            
            T.insert('1.0',output)

            response    =   tkMessageBox.askyesno("Save", "Do you want to save report ?")
            if response == True:
                f_report = asksaveasfile(mode='w', defaultextension=".txt")
                if f_report is None: # asksaveasfile return `None` if dialog closed with "cancel".
                    pass
                else:
                    f_report.write(output)
                    f_report.close()
        except:
            pass

def signFile():
    filename    =   askopenfilename()
    if (filename != "" ):
        try:
            output      =   subprocess.Popen(["./antiv", "sign", filename], stdin=subprocess.PIPE, stderr=subprocess.PIPE, stdout=subprocess.PIPE).communicate()[0]
            #tkMessageBox.showinfo("Sign File Report", output)
            r= re.compile("\033\[[0-9;]+m")
            output = r.sub("",output)
            another_window = Tk()
            s = Tkinter.Scrollbar(another_window)
            T = Tkinter.Text(another_window)

            T.focus_set()
            s.pack(side=Tkinter.RIGHT, fill=Tkinter.Y)
            T.pack(side=Tkinter.LEFT, fill=Tkinter.Y)
            s.config(command=T.yview)
            T.config(yscrollcommand=s.set)
            
            T.insert('1.0',output)
        except:
            pass

def scanFolder():
    folder  =   askdirectory()
    if(folder != ""):
        try:
            output      =   subprocess.Popen(["./antiv","scanfolder",folder], stdin=subprocess.PIPE,stderr=subprocess.PIPE,stdout=subprocess.PIPE).communicate()[0]
            r= re.compile("\033\[[0-9;]+m")
            output = r.sub("",output)
            another_window = Tk()
            s = Tkinter.Scrollbar(another_window)
            T = Tkinter.Text(another_window)

            T.focus_set()
            s.pack(side=Tkinter.RIGHT, fill=Tkinter.Y)
            T.pack(side=Tkinter.LEFT, fill=Tkinter.Y)
            s.config(command=T.yview)
            T.config(yscrollcommand=s.set)
            
            T.insert('1.0',output)

            #tkMessageBox.showinfo("Scan Report", output)
            response    =   tkMessageBox.askyesno("Save", "Do you want to save report ?")
            if response == True:
                f_report = asksaveasfile(mode='w', defaultextension=".txt")
                if f_report is None: # asksaveasfile return `None` if dialog closed with "cancel".
                    pass
                else:
                    f_report.write(output)
                    f_report.close()

        except:
            pass


top.title("Virus Detector")
top.geometry("500x500") 
top.resizable(0, 0) 
bg_image        =   PhotoImage(file="images/bg42.png")
bg_label        =   Label(top, image=bg_image)
bg_label.place(x=0, y=0, relwidth=1, relheight=1)

img             =   PhotoImage(file="images/Scan3.png")
button_scan     =   Tkinter.Button(text = "Scan File", command = scanFile,highlightthickness = 0, bd = 0, relief=FLAT)
button_scan.grid(row=0,column=1)
button_scan.config(image=img)
button_scan["border"]="0"
button_scan["bg"]="black"

img_scan_folder        =   PhotoImage(file="images/ScanFolder3.png")
button_scan_folder     =   Tkinter.Button(text = "Scan Folder", command = scanFolder,highlightthickness = 0, bd = 0, relief=FLAT)
button_scan_folder.config(image=img_scan_folder)
button_scan_folder.grid(row=1,column=1)
button_scan_folder["border"]="0"
button_scan_folder["bg"]="black"


img_sign        =   PhotoImage(file="images/Sign3.png")
button_sign     =   Tkinter.Button(text = "Sign File", command = signFile,highlightthickness = 0, bd = 0)
button_sign.config(image=img_sign)
button_sign.grid(row=2,column=1)
button_sign["border"]="0"
button_sign["bg"]="black"

img_quit        =   PhotoImage(file="images/Quit2.png")
button_quit     =   Tkinter.Button(text="Quit",command=top.destroy,highlightthickness = 0, bd = 0)
button_quit.config(image=img_quit)
button_quit.grid(row=5,column=1,pady=275)
button_quit["border"]="0"
button_quit["bg"]="black"

top.mainloop()

