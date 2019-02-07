# Python 2
#RCC = $(HOME)/.local/lib/python2.7/site-packages/PySide2/pyside2-rcc
#UIC = $(HOME)/.local/lib/python2.7/site-packages/PySide2/scripts/uic.py

# Python 3
RCC = $(HOME)/.local/lib/python3.6/site-packages/PySide2/pyside2-rcc
UIC = $(HOME)/.local/lib/python3.6/site-packages/PySide2/scripts/uic.py

all:	mdieditor_rc.py

mdieditor_rc.py: mdieditor.qrc
	$(RCC) mdieditor.qrc >mdieditor_rc.py

check: flake8

pep8: flake8

flake8:
	flake8 --max-line-length=132 mdieditor.py

distclean: clean

clean:
	$(RM) *_rc.py *.pyc
	$(RM) -rf __pycache__

