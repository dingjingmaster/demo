#!/usr/bin/env python
# -*- coding=utf-8 -*-
dict1 = locals()
strInfos = dict1['p1']
            
PhoneNumberHead = (
    '010', '020', '021', '022', '023',
    '024', '025', '027', '028', '029',
    '031', '033', '034', '035', '037',
    '039', '041', '042', '043', '045',
    '046', '047', '048', '051', '052',
    '053', '054', '055', '056', '057',
    '058', '059', '063', '066', '069',
    '070', '071', '072', '073', '074',
    '075', '076', '077', '079', '081',
    '082', '083', '085', '087', '088',
    '089', '090', '091', '093', '094',
    '095', '097', '099')


if (strInfos[0] == '('): 
    strPhoneNumberHead = strInfos[1:4]
else:
    strPhoneNumberHead = strInfos[0:3]

bRet = strPhoneNumberHead in PhoneNumberHead
if (False == bRet):
    raise Exception('Check Failed')
