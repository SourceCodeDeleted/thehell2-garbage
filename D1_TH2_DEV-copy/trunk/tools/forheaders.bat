@rem forfiles /P ��������_���� /M �����_������ /S /� "�������_���_�����"
set awk=%2
forfiles /P %1 /M *.h /S /C "cmd /c defguard.bat @file @fname"

