#include "python-packer.h"
#include <iostream>

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    PythonPacker* py = PythonPacker::instance ();

#if 1
    std::string pythonStr = ""
"# -*- coding: UTF-8 -*-"
"exit (3)"
""
"dict1 = locals()"
"strInfos = dict1['p1']"
""
"PhoneNumberHead = ("
"'010', '020', '021', '022', '023',"
"'024', '025', '027', '028', '029',"
"'031', '033', '034', '035', '037',"
"'039', '041', '042', '043', '045',"
"'046', '047', '048', '051', '052',"
"'053', '054', '055', '056', '057',"
"'058', '059', '063', '066', '069',"
"'070', '071', '072', '073', '074',"
"'075', '076', '077', '079', '081',"
"'082', '083', '085', '087', '088',"
"'089', '090', '091', '093', '094',"
"'095', '097', '099')"
"exit (3)"
"if (strInfos[0] == '('):"
"    strPhoneNumberHead = strInfos[1:4]"
"else:"
"    strPhoneNumberHead = strInfos[0:3]"
"bRet = strPhoneNumberHead in PhoneNumberHead"
"if (False == bRet):"
"    raise Exception('Check Failed')"
"";
#else
    std::string pythonStr = "\
# -*- coding: UTF-8 -*-\n\
exit (3)\n\
\n\
dict1 = locals()\n\
strInfos = dict1['p1']\n\
\n\
PhoneNumberHead = (\n\
    '010', '020', '021', '022', '023',\n\
    '024', '025', '027', '028', '029',\n\
    '031', '033', '034', '035', '037',\n\
    '039', '041', '042', '043', '045',\n\
    '046', '047', '048', '051', '052',\n\
    '053', '054', '055', '056', '057',\n\
    '058', '059', '063', '066', '069',\n\
    '070', '071', '072', '073', '074',\n\
    '075', '076', '077', '079', '081',\n\
    '082', '083', '085', '087', '088',\n\
    '089', '090', '091', '093', '094',\n\
    '095', '097', '099')\n\
exit (3)\n\
if (strInfos[0] == '('):\n\
    strPhoneNumberHead = strInfos[1:4]\n\
else:\n\
    strPhoneNumberHead = strInfos[0:3]\n\
bRet = strPhoneNumberHead in PhoneNumberHead\n\
if (False == bRet):\n\
    raise Exception('Check Failed')\n\
";

#endif

    pythonStr = "#-*- coding:utf-8 -*-\nprint 1";

    std::string pythonParam1 = "09981889123";       // 正确
    std::string pythonParam2 = "09881889123";       // 错误

    //std::cout << true << std::endl;
    //std::cout << false << std::endl;

    std::cout << pythonStr << std::endl;

    std::cout << (py->RunPythonString (pythonStr, pythonParam1) ? "true" : "false") << std::endl;
    std::cout << (py->RunPythonString (pythonStr, pythonParam2) ? "true" : "false") << std::endl;

    return 0;
}
