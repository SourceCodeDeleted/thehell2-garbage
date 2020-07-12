#coding=utf-8
import sys, os
#��������:
#1. ��������� ������ ����
#2. ���������� ����� �� ��� ������� ���������� �� "//----- ("
#3. ����� ����� � �������, ���������� ������ � ����� ����� 
#(����������� �� ��� ��� ������ ���������� � "{" � ����� � "}" � ������ ������, ��� ��������)
#4. ���������� ����� � ��������� � �������
#5. ��������� �� ����� �����
#6. �� �� ����� � ������ ������ � ������ ��������
#7. ������� ���������� ��� ������� ��������� � ����� ������. ��������� ��������� ��������
#"�����	�������1�����	�������2�����	����������"
#8. ����� �� �� ����� ����������� ��� ����������

def MakeFunctMap (content):
	functMap = {}
	#2. ���������� ����� �� ��� ������� ���������� �� "//----- ("
	findPos = content.find("\n//----- (")
	while (findPos != -1):
		# ����� ������� ��������� ������ ������
		key = content[findPos+10:findPos+18]
		#3. ����� ����� � �������, ���������� ������ � ����� ����� 
		#(����������� �� ��� ��� ������ ���������� � "{" � ����� � "}" � ������ ������, ��� ��������)
		startPos = content.find("\n{", findPos)
		endPos = content.find("\n}", findPos)
		stringsCount = content[startPos:endPos].count('\n')
		functMap[key] = {'key': key, 'startPos': startPos, 'endPos': endPos, 'stringsCount': stringsCount}
		findPos = content.find("\n//----- (", findPos+1)
	return functMap

def FunctinsCompare (functMap1, functMap2, file1, file2):
	header = "�����\t�������1�����\t�������2�����\t����������\t����� ����� � ����� 1\t����� ����� � ����� 2\n"
	overlap = []
	noOverlap = []
	for key in functMap1:
		if (functMap2.has_key(key)):
			overlap.append(key)
		else:
			noOverlap.append(key)
			
	for key in functMap2:
		if (not(functMap1.has_key(key))):
			noOverlap.append(key)
	
	print "overlap functions count = ", len(overlap)
	print "noOverlap functions count = ", len(noOverlap)
	
	result = header
	
	for key in overlap:
		function1Info = functMap1[key]
		function1Body = file1[function1Info['startPos']:function1Info['endPos']]
		function2Info = functMap2[key]
		function2Body = file2[function2Info['startPos']:function2Info['endPos']]
		isFunctionEqual = (function1Body == function2Body)
		result += "%s\t������\t������\t" % key
		
		if (isFunctionEqual):
			result += "������\t"
		else:
			result += "����\t"
			
		result += "%i\t%i\n" % (function1Info['stringsCount'], function2Info['stringsCount'])
	
	for key in noOverlap:
		result += "%s\t" % key
		if (functMap1.has_key(key)):
			result += "������\t����\t����\t%i\t0\n" % (function1Info['stringsCount'])
		else:
			result += "����\t������\t����\t0\t%i\n" % (function2Info['stringsCount'])

	return result
	
def main ():
	if (len(sys.argv) < 4):
		print "Too few arguments\nUse: firstFileName\tsecondFileName\toutputFileName"
		return
	elif (len(sys.argv) > 4):
		print "Too many arguments\nUse: firstFileName\tsecondFileName\toutputFileName"
		return
	firstFileName = sys.argv[1]
	secondFileName = sys.argv[2]
	outputFileName = sys.argv[3]

	print firstFileName, secondFileName, outputFileName
	
	f = open(firstFileName, "rt")
	firstFile = f.read()
	f.close()
	print "Open %s\nLen = %d kb" % (firstFileName, len(firstFile)/1024)
	
	functMap = MakeFunctMap(firstFile)
	print "Count of functions:", len(functMap)

	f = open(secondFileName, "rt")
	secondFile = f.read()
	f.close()
	print "Open %s\nLen = %d kb" % (secondFileName, len(secondFile)/1024)
	
	functMap2 = MakeFunctMap(secondFile)
	print "Count of functions:", len(functMap2)
	
	outputContent = FunctinsCompare (functMap, functMap2, firstFile, secondFile)
	f = open(outputFileName, "wt")
	f.write(outputContent)
	f.close()
	
main()