import sys
from math import log,sqrt,e
from datetime import datetime
import random
import time

import numpy

reload(sys)
sys.setdefaultencoding('utf8')

def timeToDays(x, tnow):
    x = [int(v) if i != 0 else v for i,v in enumerate(x)]
    uid,iid,action,stime,utype,depth,utime = x[:7]
    t = utime if utime < tnow and utime > stime else stime
    #t = stime
    t = tnow - t
    if t < 0 or t > 1095 * 86400:
        t = 1095 * 86400
    return uid,iid,t,utype,depth

def genNorIid(x, iidInfoDictB):
    uid,iid,days,utype,depth = x
    if iid not in iidInfoDictB.value:
        return
    norIid = iidInfoDictB.value[iid][7]
    yield uid,(norIid,days,utype,depth)

def uniqNorIid(x):
    uid,ilist = x
    iidDict = {}
    for iid,days,utype,depth in ilist:
        if iid in iidDict:
            if days < iidDict[iid][0]:
                iidDict[iid][0] = days
            if depth > iidDict[iid][2]:
                iidDict[iid][2] = depth
        else:
            iidDict[iid] = [days,utype,depth]
    uniqList = []
    for iid,(days,utype,depth) in iidDict.items():
        uniqList.append((iid,days,utype,depth))
    return uid,uniqList

def genIidCount(x):
    uid,ilist = x
    for iid,days,utype,depth in ilist:
        if days < 365 * 86400 and depth > 1:
            yield iid,1

def sortByDays(x):
    uid,ilist = x
    sortList = sorted(ilist, key=lambda x:x[1])
    return uid,sortList

def genIidPair(x, norIidInfoDictB):
    uid,ilist = x
    sortList = []
    for iid,days,utype,depth in ilist:
        if days < 365 * 86400 and depth > 1:
            sortList.append((iid,days))
    size = len(sortList)
    for i in range(size):
        iid1,days1 = sortList[i]
        for j in range(i+1, size):
            iid2,days2 = sortList[j]
            if iid1 < iid2:
                yield (iid1,iid2),(1,1/log(1+float(len(sortList))))
                #yield (iid1,iid2),1/((1+0.001*abs(days1-days2))*log(1+float(len(sortList))))
            else:
                yield (iid2,iid1),(1,1/log(1+float(len(sortList))))

def genCosine(x, clickDictB, iidInfoDictB, norIidInfoDictB, cpWeightDictB, baseCount):
    (iid1,iid2),(count,score) = x
    n1 = clickDictB.value[iid1]
    n2 = clickDictB.value[iid2]
    if n1 >= 3 and n2 >= 3 and count >= 3:
        sim = score / (sqrt(n1 * n2))
        selDict1 = norIidInfoDictB.value[iid1][0]
        selDict2 = norIidInfoDictB.value[iid2][0]
        for iidsrc,selDict in ((iid2,selDict1),(iid1,selDict2)):
            for charge,(iid,viewCount,rt) in selDict:
                if charge >= 1 or viewCount >= 100:
                    iidInfo = iidInfoDictB.value[iid]
                    cp,tweight = iidInfo[8],iidInfo[11]
                    cpw = 1
                    if cp in cpWeightDictB.value:
                        cpw = cpWeightDictB.value[cp]
                    #yield iidsrc,(iid,cpw*rt*sim/(viewCount+5000),cpw*rt*sim/(viewCount+50000),cpw*rt*sim/(viewCount+150000),count,charge)
                    yield iidsrc,(iid,tweight*sim,cpw*rt*sim,cpw*rt*sim,count,charge)

def sortSimList(ilist, iidInfoDictB, debugDict, j, libCount):
    sortList = sorted(ilist, key=lambda x: x[j], reverse=True)
    freeList,monthList,chargeList,limitList,fcList,fcFreeList,fcLimitList = [],[],[],[],[],[],[]
    for i in (freeList,monthList,chargeList,limitList,fcList,fcFreeList,fcLimitList):
        for k in range(libCount):
            i.append([])
    for i in range(len(sortList)):
        addFlag = False
        iid2,score1,score2,score3,count,charge = sortList[i]
        score = eval('score'+str(j))
        iidInfo = iidInfoDictB.value[iid2]
        cp,tf,by,fc,ci,ii,mk = iidInfo[8],iidInfo[9],iidInfo[15],iidInfo[18],iidInfo[19],iidInfo[20],iidInfo[21]
        for k in range(libCount):
            if mk[k] == '1':
                continue
            if (tf[k] == '0' or fc[k] == '1' or ii[k] == '1') and len(freeList[k]) < 300:
                freeList[k].append((iid2,score))
                addFlag = True
            if by[k] == '1' and len(monthList[k]) < 300:
                monthList[k].append((iid2,score))
                addFlag = True
            if (by[k] == '1' or ci[k] == '1' or tf[k] != 'n' or fc[k] == '1') and len(chargeList[k]) < 300:
                chargeList[k].append((iid2,score))
                addFlag = True
            if tf[k] == '0' and len(limitList[k]) < 300:
                limitList[k].append((iid2,score))
                addFlag = True
            if fc[k] == '1' and len(fcList[k]) < 300:
                fcList[k].append((iid2,score))
                addFlag = True
            if (tf[k] == '0' or fc[k] == '1') and cp == 1000001 and len(fcFreeList[k]) < 300:
                fcFreeList[k].append((iid2,score))
                addFlag = True
            if (tf[k] == '0' or fc[k] == '1') and len(fcLimitList[k]) < 300:
                fcLimitList[k].append((iid2,score))
                addFlag = True
            if debugDict != None and addFlag and k == 0:
                debugDict[iid2] = count
        breakFlag = True
        for k in range(libCount):
            if len(freeList[k]) < 300 or len(monthList[k]) < 300 or len(chargeList[k]) < 300 or len(limitList[k]) < 300 or\
               len(fcList[k]) < 300 or len(fcFreeList[k]) < 300 or len(fcLimitList[k]) < 300:
                breakFlag = False
        if breakFlag:
            break
    return freeList,monthList,chargeList,limitList,fcList,fcFreeList,fcLimitList

def sortSim(x, iidInfoDictB, debugFlag, libCount):
    iid,ilist = x
    freeLists,monthLists,chargeLists,limitLists,fcLists,fcFreeLists,fcLimitLists = [],[],[],[],[],[],[]
    debugDict = {} if debugFlag else None
    for i in range(1):
        freeList,monthList,chargeList,limitList,fcList,fcFreeList,fcLimitList = sortSimList(ilist, iidInfoDictB, debugDict, i+1, libCount)
        freeLists.append(freeList)
        monthLists.append(monthList)
        chargeLists.append(chargeList)
        limitLists.append(limitList)
        fcLists.append(fcList)
        fcFreeLists.append(fcFreeList)
        fcLimitLists.append(fcLimitList)
    return iid,freeLists,monthLists,chargeLists,limitLists,fcLists,fcFreeLists,fcLimitLists,debugDict

def uniqSimList(simList, iidInfoDictB, norName, t, count=128):
    serNameSet = set()
    uniqList = []
    maxScore = None
    for iid,score in simList:
        v = iidInfoDictB.value[iid]
        norName2,serName,status,charge,utime = v[1],v[2],v[3],v[5],v[12]
        if status != 2 and charge >= 1 and utime < t:
            continue
        if norName == norName2 or serName in serNameSet:
            continue
        if not maxScore:
            maxScore = score
        serNameSet.add(serName)
        uniqList.append((iid,score/maxScore))
        if len(uniqList) == count:
            break
    return uniqList

def uniqSimLists(simLists, iidInfoDictB, norName, t, k, count=128):
    uniqLists = []
    for simList in simLists:
        uniqLists.append(uniqSimList(simList[k], iidInfoDictB, norName, t, count))
    serNameDict = {}
    uniqList = []
    size = max([len(i) for i in uniqLists])
    for i in range(size):
        for j in range(3):
            if len(uniqLists[j]) <= i:
                continue
            iid,score = uniqLists[j][i]
            serName = iidInfoDictB.value[iid][2]
            if serName in serNameDict:
                pos = serNameDict[serName]
                if score > uniqList[pos][1]:
                    uniqList[pos][1] = score
                continue
            uniqList.append([iid,score])
            serNameDict[serName] = len(uniqList) - 1
        if len(uniqList) >= count:
            break
    return uniqList[:count]

def uniqSim(x, iidInfoDictB, t, libCount):
    iid,debugDict = x[0],x[8]
    simLists = x[1:8]
    norName = iidInfoDictB.value[iid][1]
    uniqLists = []
    for i in range(len(simLists)):
        uniqLists.append([])
        for j in range(libCount):
            uniqLists[i].append([])
            k = 0# if j % 3 == 0 else 1
            uniqLists[i][j] = uniqSimList(simLists[i][k][j], iidInfoDictB, norName, t)
    freeList,monthList,chargeList,limitList,fcList,fcFreeList,fcLimitList = uniqLists
    returnFlag = True
    for i in range(len(simLists)):
        for j in range(libCount):
            if uniqLists[i][j]:
                returnFlag = False
    if returnFlag:
        return
    yield iid,freeList,monthList,chargeList,limitList,fcList,fcFreeList,fcLimitList,debugDict

def mapSim(x, norIidInfoDictB):
    iid,freeList,monthList,chargeList,limitList,fcList,fcFreeList,fcLimitList,debugDict = x
    iidList = norIidInfoDictB.value[iid][1]
    for iid2 in iidList:
        yield iid2,freeList,monthList,chargeList,limitList,fcList,fcFreeList,fcLimitList,debugDict

def countSim(x, iidInfoDictB):
    iid,freeList,monthList,chargeList,limitList,debugDict = x
    iidInfo = iidInfoDictB.value[iid]
    viewCount,charge = iidInfo[4],iidInfo[5]
    count = len(str(viewCount)) if viewCount != 0 else 0
    #count = viewCount/10000
    fcount = 0
    ccount = 0
    fcountList = numpy.array([0] * 10)
    ccountList = numpy.array([0] * 10)
    for iid2,sim in freeList[:10]:
        viewCount = iidInfoDictB.value[iid2][4]
        fcount += viewCount
        fcountList[len(str(viewCount))] += 1
    for iid2,sim in chargeList[:10]:
        viewCount = iidInfoDictB.value[iid2][4]
        ccount += viewCount
        ccountList[len(str(viewCount))] += 1
    if fcount:
        yield (charge,0,count),(1,fcount,fcountList)
    if ccount:
        yield (charge,1,count),(1,ccount,ccountList)

def genRate(x, clickDictB, simDictB, days, feature):
    uid,sortList = x
    #if sortList[0][1] > days:
     #   return
    ilist = []
    i = 0
    ukey = None
    for iid,days,utype,depth in sortList:
        if iid not in simDictB.value:# or iid not in clickDictB.value:
            continue
        rate = 1/log(e+days/86400.0)
        #if days < 30 and depth >= feature:
         #   rate *= 10
        #rate = 1/log(e+feature*float(clickDictB.value[iid]))
        #rate = 1/((1+0.001*days)*log(1+float(clickDictB.value[iid])))
        freeList,monthList,chargeList,limitList = simDictB.value[iid]
        ilist.append((iid,days,depth,rate,freeList,monthList,chargeList,limitList))
        i += 1
        ukey = utype
    if True:#i >= 3 and ilist[2][1] <= 30:
        yield uid,ilist,ukey

def genRate2(x, clickDictB):
    iid,(ulist,simList) = x
    rate = 1
    #rate = 1/((1+0.001*days)*log(1+float(clickDictB.value[iid])))
    for uid,days in ulist:
        yield uid,(iid,days,rate,simList)

def calScore(iid1, rate, simList, scoreDict, clickSet, k, debugFlag):
    i = 0
    for iid2,sim in simList:
        if iid2 in clickSet:
            continue
        score = rate * sim
        node = scoreDict.setdefault(iid2,[0,[]])
        node[0] += score
        if debugFlag:
            node[1].append((iid1,score))
        i += 1
        if i == k:
            break

def getSortList(scoreDict, iidInfoDictB=None, debugDict=None):
    sortList = sorted(scoreDict.items(), key = lambda x: x[1][0], reverse = True)
    maxScore = sortList[0][1][0]
    recList = []
    batRecList = [[],[],[],[],[],[],[]]
    for iid,(score,scoreList) in sortList[:1000]:
        score = score/maxScore
        recList.append((iid,score))
        if debugDict != None:
            scoreList.sort(key=lambda x: x[1], reverse=True)
            debugDict[iid] = scoreList
        if iidInfoDictB and iidInfoDictB.value[iid][6][4] == '1':
            batRecList[4].append((iid,score))
    return recList,batRecList[1:]

def getRecList(ilist, clickSet, k, debugFlag, iidInfoDictB, ukey):
    freeDict,chargeDict,monDict,limitDict = {},{},{},{}
    newFlag = False
    if len(ilist) >= 3 and ilist[2][1] <= 30:
        newFlag = True
    tdays = 30
    subsList = filter(lambda x: x[2] == 3, ilist)
    count = 10 if ukey >= 5 else 5
    for iid,days,depth,rate,freeList,monthList,chargeList,limitList in subsList[:5]:
        if days > tdays * 86400:
            continue
        calScore(iid, rate, freeList, freeDict, clickSet, k, debugFlag)
        calScore(iid, rate, monthList, monDict, clickSet, k, debugFlag)
        calScore(iid, rate, chargeList, chargeDict, clickSet, k, debugFlag)
        calScore(iid, rate, limitList, limitDict, clickSet, k, debugFlag)
    freeList,monthList,chargeList,limitList = [],[],[],[]
    batRecList = [[]] * 6
    debugDict = {}
    if len(freeDict) != 0:
        freeList,tmpList = getSortList(freeDict)
    if len(monDict) != 0:
        monthList,tmpList = getSortList(monDict)
    if len(chargeDict) != 0:
        chargeList,batRecList = getSortList(chargeDict, None, debugDict)
    if len(limitDict) != 0:
        limitList,tmpList = getSortList(limitDict)
    return freeList,monthList,chargeList,limitList,batRecList,debugDict
    
def rec(x, k, days, debugFlag, iidInfoDictB):
    uid,ilist,ukey = x
    if not ilist:
        return
    ilist = list(ilist)
    clickList = []
    clickSet = set()
    for iid,days,depth,rate,freeList,monthList,chargeList,limitList in ilist:
        clickList.append((iid,days,depth))
        clickSet.add(iid)
    freeList,monthList,chargeList,limitList,batRecList,debugDict = getRecList(ilist, clickSet, k, debugFlag, iidInfoDictB, ukey)
    if True:#freeList or monthList or chargeList or limitList:
        yield uid,freeList,monthList,chargeList,limitList,batRecList,clickList,debugDict,ukey

def getUserType(ftlist):
    ftdict = {}
    for ft in ftlist:
        ftdict.setdefault(ft, 0)
        ftdict[ft] += 1
    return sorted(ftdict.items(), key=lambda x:x[1], reverse=True)[0][0]

def uniqRecList(recList, iidInfoDictB, norNameSet, count, ut, idx=0, cpdict=None, tflag=False):
    serNameSet = set()
    uniqList = []
    for iid,score in recList:
        if iid not in iidInfoDictB.value:
            continue
        v = iidInfoDictB.value[iid]
        norName,serName,charge,cp,tf,ft,by,fc = v[1],v[2],v[5],v[8],v[9],v[10],v[15],v[18]
        #if by[idx] != '1' and ci[idx] != '1' and tf[idx] == 'n' and fc[idx] != '1':
         #   continue
        if ut and ft != ut:
            continue
        if norName in norNameSet or serName in serNameSet:
            continue
        if cpdict:
            cp = 1 if cp == 1000001 else 0
            if cpdict[cp][0] == cpdict[cp][1]:
                continue
            cpdict[cp][0] += 1
        serNameSet.add(serName)
        uniqList.append((iid,score))
        if len(uniqList) >= count:
            break
    return uniqList

def addFeature(recList, featureDictB, iidInfoDictB):
    sortList = []
    for iid,score in recList:
        rs,rt = 0,0
        if iid in featureDictB.value:
            rs = featureDictB.value[iid]
        if iid in iidInfoDictB.value:
            rt = iidInfoDictB.value[iid][13]
        score = score + 2.0 * rs
        sortList.append((iid,score))
    sortList.sort(key=lambda x: -x[1])
    return sortList
    
def uniqRec(x, iidInfoDictB, debugFlag, featureDictB):
    uid,freeList,monthList,chargeList,limitList,batRecList,clickList,debugDict,ukey = x
    norNameSet = set()
    ftlist = []
    for iid,days,depth in clickList:
        if iid not in iidInfoDictB.value:
            continue
        v = iidInfoDictB.value[iid]
        norName,ft = v[1],v[10]
        norNameSet.add(norName)
        ftlist.append(ft)
    ut = getUserType(ftlist)
    freeList = uniqRecList(freeList, iidInfoDictB, norNameSet, 64, None)
    monthList = uniqRecList(monthList, iidInfoDictB, norNameSet, 64, None)
    #chargeList = uniqRecList(chargeList, iidInfoDictB, norNameSet, 128, None, {0:[0,38], 1:[0,90]})
    chargeList2 = addFeature(chargeList, featureDictB, iidInfoDictB)
    chargeList3 = uniqRecList(chargeList2, iidInfoDictB, norNameSet, 32, None)
    limitList = uniqRecList(limitList, iidInfoDictB, norNameSet, 64, ut)
    if False:#ukey >= 5:
        limitList = chargeList3
        chargeList3 = uniqRecList(chargeList2, iidInfoDictB, norNameSet, 32, None)
    chargeList = chargeList3
    batUniqList = []
    for rlist in batRecList:
        ulist = uniqRecList(rlist, iidInfoDictB, norNameSet, 32, None, {0:[0,0], 1:[0,32]})
        batUniqList.append(ulist)
    if True:#freeList or monthList or chargeList or limitList:
        if not debugFlag:
            clickList = []
        yield uid,freeList,monthList,chargeList,limitList,batUniqList,clickList,debugDict

def countRec(x, iidInfoDictB):
    uid,freeList,monthList,chargeList,limitList,batUniqList,clickList,debugDict = x
    fcount = 0
    ccount = 0
    fcountList = numpy.array([0] * 10)
    ccountList = numpy.array([0] * 10)
    for iid2,sim in freeList[:10]:
        viewCount = iidInfoDictB.value[iid2][4]
        fcount += viewCount
        fcountList[len(str(viewCount))] += 1
    for iid2,sim in chargeList[:10]:
        viewCount = iidInfoDictB.value[iid2][4]
        ccount += viewCount
        ccountList[len(str(viewCount))] += 1
    if fcount:
        yield 0,(1,fcount,fcountList)
    if ccount:
        yield 1,(1,ccount,ccountList)

def diffRec(x):
    uid,freeList,monthList,chargeList,limitList,batUniqList,clickList,debugDict = x
    ilist1 = [iid for iid,score in chargeList]
    ilist2 = [iid for iid,score in limitList]
    def calCount(ilist1, ilist2):
        set1 = set(ilist1)
        set2 = set(ilist2)
        return len(set1.intersection(set2))
    count1 = calCount(ilist1[:10],ilist2[:10])
    count2 = calCount(ilist1[:32],ilist2[:32])
    return 10,count1,32,count2

class ItemCF:
    def __init__(self, sc, data, iidInfoData, uidInfo=None):
        self.libCount = None
        self.sc = sc
        tnow = time.time()
        self.iidInfoDictB,self.norIidInfoDictB = self.loadIidInfo(iidInfoData, tnow)
        iidInfoDictB = self.iidInfoDictB
        self.data = data.map(lambda x: timeToDays(x,tnow))\
                        .flatMap(lambda x: genNorIid(x,iidInfoDictB))\
                        .groupByKey()\
                        .map(uniqNorIid)\
                        .filter(lambda x: len(x[1]) < 1000)\
                        #.cache()
        self.clickDict = {}
        for iid,count in self.data.flatMap(genIidCount)\
                                  .reduceByKey(lambda x,y: x+y) \
                                  .collect():
            self.clickDict[iid] = count
        self.clickDictB = self.sc.broadcast(self.clickDict)
        self.simData = None
        self.simDict = {}
        self.simDictB = None
        self.mapSimData = None
        self.uidInfo = uidInfo

    def loadIidInfo(self, iidInfoData, tnow):
        iidInfoDictB = None
        norIidInfoDictB = None
        libCountSet = set()
        if iidInfoData:
            keyDict = {}
            iidInfoDict = {}
            norIidInfoDict = {}
            bystat = [0] * 100
            cistat = [0] * 100
            tfstat = [0] * 100
            fcstat = [0] * 100
            iistat = [0] * 100
            for i in iidInfoData.collect():
                if len(i) < 33:
                    continue
                iid,name,norName,serName,status,viewCount,charge,filters,norAuthor,cp,tf,ft,itime,utime,rt,rn,by,ai,fc,ci,ii,maskLevel = int(i[0]),i[1],i[2],i[10],int(i[9]),int(i[8]),int(i[12]),i[20],i[7],int(i[15]),i[22],i[5],int(i[17]),int(i[18]),float(i[27]),int(i[28]),i[25],i[26],i[29],i[30],i[31],i[32]
                if norName == '' or norAuthor == '':
                    continue
                by = by.replace(',','')
                ci = ci.replace(',','')
                tf = tf.replace(',','')
                fc = fc.replace(',','')
                ii = ii.replace(',','')
                maskLevel = maskLevel.replace(',','')
                for i in (by, ci, tf, fc, maskLevel):
                    libCountSet.add(len(i))
                rt = (rt*rn+1)/(rn+10)
                t = tnow - itime
                if t > 3 * 365 * 86400:
                    t = 3 * 365 * 86400
                elif t < 0:
                    t = 0
                tweight = 1.0/(1+t/(365*86400))
                ftList = ft.split(',')
                ft = ftList[0]
                st = ftList[1] if len(ftList) > 2 else ''
                key = norName + norAuthor
                if key not in keyDict:
                    keyDict[key] = [iid,0,{},[]]
                norIid,fstatus,selDict,iidList = keyDict[key]
                iidList.append(iid)
                if charge not in selDict or viewCount > selDict[charge][1]:
                    selDict[charge] = [iid,viewCount,rt]
                iidInfoDict[iid] = name,norName,serName,status,viewCount,charge,filters,norIid,cp,tf,ft,tweight,utime,rt,rn,by,ai,st,fc,ci,ii,maskLevel
                for i in range(len(maskLevel)):
                    if maskLevel[i] == '1':
                        continue
                    if by[i] == '1' and maskLevel[i] == '0':
                        bystat[i] += 1
                    if ci[i] == '1' and maskLevel[i] == '0':
                        cistat[i] += 1
                    if tf[i] != 'n' and maskLevel[i] == '0':
                        tfstat[i] += 1
                    if fc[i] == '1' and maskLevel[i] == '0':
                        fcstat[i] += 1
                    if ii[i] == '1' and maskLevel[i] == '0':
                        iistat[i] += 1
            if len(libCountSet) != 1:
                print 'lib err',len(libCountSet)
                raise
            self.libCount = list(libCountSet)[0]
            print 'lib count',self.libCount
            for stat in (bystat,cistat,tfstat,fcstat,iistat):
                for i in range(self.libCount):
                    print stat[i],
                print ''
            #ratio = self.calAvgRate(iidInfoDict)
            ratio = 1.0
            iidInfoDictB = self.sc.broadcast(iidInfoDict)
            for key,[iid,fstatus,selDict,iidList] in keyDict.items():
                if 0 in selDict and (fstatus == -1 or 10 in selDict):
                    del selDict[0]
                if 1 in selDict and 10 in selDict:
                    del selDict[1]
                if 2 in selDict:
                    selDict[2][2] /= ratio
                norIidInfoDict[iid] = selDict.items(),iidList
            norIidInfoDictB = self.sc.broadcast(norIidInfoDict)
        return iidInfoDictB,norIidInfoDictB

    def calAvgRate(self, iidInfoDict):
        stDict = {}
        texpire = time.time() - 30 * 86400
        for iid,v in iidInfoDict.items():
            status,charge,utime,rt,rn,by = v[3],v[5],v[12],v[13],v[14],v[15]
            if status < 0 or (charge >= 1 and status != 2 and utime < texpire):
                continue
            rn2 = int(rt * rn)
            if by[0] == '1':
                charge = 2
            stDict.setdefault(charge, numpy.array([0]*3))
            stDict[charge] += [rn,rn2,1]
        avgDict = {}
        for charge,(rn,rn2,count) in stDict.items():
            avgDict[charge] = float(rn2)/rn
            print '%s %d %d %d %f' % (charge, count, rn, rn2, float(rn2)/rn)
        return avgDict[2]/avgDict[1]

    def printData(self, data, countFunc=countSim):
        iidInfoDictB = self.iidInfoDictB
        for i in data.flatMap(lambda x: countFunc(x, iidInfoDictB)) \
                     .reduceByKey(lambda x,y: (x[0]+y[0],x[1]+y[1],x[2]+y[2])) \
                     .sortByKey() \
                     .collect():
            k,(lcount,vcount,larr) = i
            rcount = sum(larr)
            avgCount = vcount/rcount if rcount != 0 else 0
            print k,'%d\t%.2f\t%d\t' % (lcount,float(rcount)/lcount,avgCount),
            for j in range(1, 8):
                rate = float(larr[j])/rcount if rcount != 0 else 0
                print '%.4f\t' % rate,
            print ''

    def getSimData(self, uniqFlag=True, baseCount=50000, debugFlag=False):
        clickDictB = self.clickDictB
        iidInfoDictB = self.iidInfoDictB
        norIidInfoDictB = self.norIidInfoDictB
        cpWeightDict = {98:1.0}
        libCount = self.libCount
        cpWeightDictB = self.sc.broadcast(cpWeightDict)
        self.simData = self.data.filter(lambda x: x[1][0][2] == 1) \
                                .flatMap(lambda x: genIidPair(x, norIidInfoDictB)) \
                                .reduceByKey(lambda x,y: (x[0]+y[0],x[1]+y[1])) \
                                .flatMap(lambda x: genCosine(x, clickDictB, iidInfoDictB, norIidInfoDictB, cpWeightDictB, baseCount)) \
                                .groupByKey() \
                                .map(lambda x: sortSim(x, iidInfoDictB, debugFlag, libCount))
        if uniqFlag and self.iidInfoDictB:
            t = time.time() - 30 * 86400
            iidInfoDictB = self.iidInfoDictB
            self.simData = self.simData.flatMap(lambda x: uniqSim(x, iidInfoDictB, t, libCount)) \
                                       .cache()
        self.mapSimData = self.simData.flatMap(lambda x: mapSim(x, norIidInfoDictB))
        #self.printData(self.mapSimData, countSim)
        #raise
        return self.mapSimData

    def loadSimData(self, path):
        def genItemList(x):
            iid = int(x[0][2:])
            recList = []
            for i in range(1, len(x), 2):
                iid2,score = int(x[i][2:]),int(x[i+1])
                recList.append((iid2,float(score)/100000))
            yield iid,([],[],[]),([],[],[]),(recList,[],[]),([],[],[]),None
        self.simData = self.sc.textFile(path)\
                              .map(lambda x: x.split('\t'))\
                              .filter(lambda x: len(x) > 1)\
                              .flatMap(genItemList)

    def recByBroad(self, k, days, debugFlag, feature=1):
        if not self.simDictB:
            for iid,freeList,monthList,chargeList,limitList,debugDict in self.simData.collect():
                self.simDict[iid] = [],[],chargeList[0],[]
            self.simDictB = self.sc.broadcast(self.simDict)
        clickDictB = self.clickDictB
        simDictB = self.simDictB
        iidInfoDictB = self.iidInfoDictB
        recData = self.data.map(sortByDays) \
                           .flatMap(lambda x: genRate(x,clickDictB,simDictB,days,feature)) \
                           .flatMap(lambda x: rec(x,k,days,debugFlag, iidInfoDictB))
        return recData

    def recByJoin(self, k, days, debugFlag):
        clickDictB = self.clickDictB
        recData = self.data.map(lambda x: (x[1],(x[0],x[2]))) \
                           .groupByKey() \
                           .join(self.simData) \
                           .flatMap(lambda x: genRate2(x,clickDictB)) \
                           .groupByKey() \
                           .flatMap(sortByDays) \
                           .flatMap(lambda x: rec(x,k,days,debugFlag))
        return recData

    def getRecData(self, days=1095, uniqFlag=True, debugFlag=False, k=128, feature=1, featureDictB=None):
        if not self.simData and not self.simDictB:
            self.getSimData(uniqFlag, debugFlag=debugFlag)
        recData = self.recByBroad(k,days,debugFlag,feature)
        if uniqFlag and self.iidInfoDictB:
            iidInfoDictB = self.iidInfoDictB
            recData = recData.flatMap(lambda x: uniqRec(x,iidInfoDictB,debugFlag,featureDictB))
        #self.printData(recData, countRec)
        #raise
        #print recData.map(diffRec)\
         #            .reduce(lambda x,y: [x[i]+y[i] for i in range(4)])
        '''
        def genTuple(x):
            chargeList,limitList,ukey = x[3],x[4],x[-1]
            ccount = 1 if len(chargeList) > 0 else 0
            lcount = 1 if len(limitList) > 0 else 0
            return ukey,(ccount,lcount)
        print recData.map(genTuple)\
                     .reduceByKey(lambda x,y: (x[0]+y[0],x[1]+y[1]))\
                     .collect()
        recData = recData.map(lambda x: x[:-1])
        '''
        return self.mapSimData,recData

def predict(indict, debugFlag=False):
    sc = indict['sc']
    data = indict['training_set']
    itemInfo = indict['item_info']
    days = 1095
    if 'days' in indict:
        days = int(indict['days'])
    stage = ''
    if 'stage' in indict:
        stage = indict['stage']
    handler = ItemCF(sc, data, itemInfo)
    outDict = {}
    if stage == 'item':
        simData = handler.getSimData(debugFlag=debugFlag)
        outDict = {'similar_items':simData}
    else:
        simData,recData = handler.getRecData(days=days, debugFlag=debugFlag)
        outDict = {'similar_items':simData,'recommendations':recData}
    return outDict
