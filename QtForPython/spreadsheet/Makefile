# Python 2
#VER=2.7

# Python 3
VER=3.6

RCC = $(HOME)/.local/lib/python${VER}/site-packages/PySide2/pyside2-rcc
UIC = $(HOME)/.local/lib/python${VER}/site-packages/PySide2/scripts/uic.py

all:	spreadsheet_rc.py ui_sortdialog.py ui_gotocelldialog.py

spreadsheet_rc.py: spreadsheet.qrc
	$(RCC) spreadsheet.qrc >spreadsheet_rc.py

ui_sortdialog.py: sortdialog.ui
	$(UIC) sortdialog.ui >ui_sortdialog.py

ui_gotocelldialog.py: gotocelldialog.ui
	$(UIC) gotocelldialog.ui >ui_gotocelldialog.py

check: flake8

pep8: flake8

flake8:
	flake8 --max-line-length=132 spreadsheet.py

distclean: clean

clean:
	$(RM) ui_*.py *_rc.py *.pyc
	$(RM) -rf __pycache__
