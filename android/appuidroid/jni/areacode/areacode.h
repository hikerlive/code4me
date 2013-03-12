#ifndef __AREACODE_H
#define __AREACODE_H

const char _area_cities[] = {
	"|10,北京|20,广州|21,上海|22,天津|23,重庆|24,辽宁沈阳|25,江苏南京|27,湖北武汉|28,四川成都|29,陕西西安" \
	"|310,河北邯郸|311,河北石家庄|312,河北保定|313,河北张家口|314,河北承德|315,河北唐山|316,河北廊坊|317,河北沧州|318,河北衡水|319,河北邢台|335,河北秦皇岛" \
	"|349,山西朔州|350,山西忻州|351,山西太原|352,山西大同|353,山西阳泉|354,山西榆次|355,山西长治|356,山西晋城|357,山西临汾|358,山西吕梁|359,山西运城|370,河南商丘|371,河南郑州|372,河南安阳|373,河南新乡|374,河南许昌|375,河南平顶山|376,河南信阳|377,河南南阳|378,河南开封|379,河南洛阳|391,河南焦作|392,河南鹤壁|393,河南濮阳|394,河南周口|395,河南漯河|396,河南驻马店|397,河南潢川|398,河南三门峡" \
	"|410,辽宁铁岭|411,辽宁大连|412,辽宁鞍山|413,辽宁抚顺|414,辽宁本溪|415,辽宁丹东|416,辽宁锦州|417,辽宁营口|418,辽宁阜新|419,辽宁辽阳|421,辽宁朝阳|427,辽宁盘锦|429,辽宁葫芦岛" \
	"|431,吉林长春|432,吉林吉林|433,吉林延吉|434,吉林四平|435,吉林通化|436,吉林白城|437,吉林辽源|438,吉林松原|439,吉林白山|440,吉林珲春|445,网络电话|448,吉林梅河" \
	"|451,黑龙江哈尔滨|452,黑龙江齐齐哈尔|453,黑龙江牡丹江|454,黑龙江佳木斯|455,黑龙江绥化|456,黑龙江黑河|457,黑龙江大兴安岭|458,黑龙江伊春|459,黑龙江大庆|464,黑龙江七台河|467,黑龙江鸡西|468,黑龙江鹤岗|469,黑龙江双鸭山"\
	"|470,内蒙古海拉尔|471,内蒙古呼和浩特|472,内蒙古包头|473,内蒙古乌海|474,内蒙古乌兰察布盟|475,内蒙古通辽|476,内蒙古赤峰|477,内蒙古伊克昭盟|478,内蒙古巴彦卓尔盟|479,内蒙古锡林浩特|482,内蒙古兴安盟|483,内蒙古阿拉山左旗"\
	"|510,江苏无锡|511,江苏镇江|512,江苏苏州|513,江苏南通|514,江苏扬州|515,江苏盐城|516,江苏徐州|517,江苏淮阴|518,江苏连云港|519,江苏常州|523,江苏泰州|527,江苏宿迁"\
	"|530,山东菏泽|531,山东济南|532,山东青岛|533,山东淄博|534,山东德州|535,山东烟台|536,山东潍坊|537,山东济宁|538,山东泰安|539,山东临沂|543,山东滨州|546,山东东营"\
	"|550,安徽滁州|551,安徽合肥|552,安徽蚌埠|553,安徽芜湖|554,安徽淮南|555,安徽马鞍山|556,安徽安庆|557,安徽宿州|558,安徽阜阳|559,安徽黄山|561,安徽淮北|562,安徽铜陵|563,安徽宣城|564,安徽六安|565,安徽巢湖|566,安徽池州"\
	"|570,浙江衢州|571,浙江杭州|572,浙江湖州|573,浙江嘉兴|574,浙江宁波|575,浙江绍兴|576,浙江台州|577,浙江温州|578,浙江丽水|579,浙江金华|580,浙江舟山"\
	"|591,福建福州|592,福建厦门|593,福建宁德|594,福建莆田|595,福建泉州|596,福建漳州|597,福建龙岩|598,福建三明|599,福建南平"\
	"|631,山东威海|632,山东枣庄|633,山东日照|634,山东莱芜|635,山东聊城"\
	"|660,广东汕尾|661,广东潮阳|662,广东阳江|663,广东揭阳|668,广东茂名"\
	"|691,云南西双版纳|692,云南德宏"\
	"|701,江西鹰潭"\
	"|710,湖北襄樊|711,湖北鄂州|712,湖北孝感|713,湖北黄冈|714,湖北黄石|715,湖北咸宁|716,湖北荆州|717,湖北宜昌|718,湖北恩施|719,湖北十堰|722,湖北随州|724,湖北荆门|728,湖北江汉"\
	"|730,湖南岳阳|731,湖南长株潭|734,湖南衡阳|735,湖南郴州|736,湖南常德|737,湖南益阳|738,湖南娄底|739,湖南邵阳|743,湖南吉首|744,湖南张家界|745,湖南怀化|746,湖南永州"\
	"|750,广东江门|751,广东韶关|752,广东惠州|753,广东梅州|754,广东汕头|755,广东深圳|756,广东珠海|757,广东佛山|758,广东肇庆|759,广东湛江|760,广东中山|762,广东河源|763,广东清远|766,广东云浮|768,广东潮州|769,广东东莞"\
	"|770,广西防城港|771,广西南宁|772,广西柳州|773,广西桂林|774,广西梧州|775,广西玉林|776,广西百色|777,广西钦州|778,广西河池|779,广西北海"\
	"|790,江西新余|791,江西南昌|792,江西九江|793,江西上饶|794,江西抚州|795,江西宜春|796,江西吉安|797,江西赣州|798,江西景德镇|799,江西萍乡"\
	"|812,四川攀枝花|813,四川自贡|816,四川绵阳|817,四川南充|818,四川达州|825,四川遂宁|826,四川广安|827,四川巴中|830,四川泸州|831,四川宜宾|832,四川内江|833,四川乐山|834,四川凉山|835,四川雅安|836,四川康定|837,四川马尔康|838,四川德阳|839,四川广元"\
	"|851,贵州贵阳|852,贵州遵义|853,贵州安顺|854,贵州都匀|855,贵州凯里|856,贵州铜仁|857,贵州毕节|858,贵州六盘水|859,贵州兴义"\
	"|870,云南昭通|871,云南昆明|872,云南大理|873,云南红河|874,云南曲靖|875,云南保山|876,云南文山|877,云南玉溪|878,云南楚雄|879,云南思茅|881,云南东川|883,云南临沧|886,云南怒江|887,云南迪庆|888,云南丽江"\
	"|890,海南儋州"\
	"|891,西藏拉萨|892,西藏日喀则|893,西藏山南|894,西藏林芝|895,西藏昌都|896,西藏那曲|897,西藏阿里"\
	"|898,海南海口|899,海南三亚"\
	"|901,新疆塔城|902,新疆哈密|903,新疆和田|906,新疆阿勒泰|908,新疆阿图什(克州)|909,新疆博乐"\
	"|910,陕西咸阳|911,陕西延安|912,陕西榆林|913,陕西渭南|914,陕西商洛|915,陕西安康|916,陕西汉中|917,陕西宝鸡|919,陕西铜川"\
	"|930,甘肃临夏|931,甘肃兰州|932,甘肃定西|933,甘肃平凉|934,甘肃庆阳|935,甘肃金昌武威|936,甘肃张掖|937,甘肃酒泉嘉峪关|938,甘肃天水|939,甘肃陇南|941,甘肃甘南|943,甘肃白银"\
	"|951,宁夏银川|952,宁夏石嘴山|953,宁夏吴忠|954,宁夏固原|955,宁夏中卫"\
	"|970,青海海晏|971,青海西宁|972,青海海东州|973,青海黄南藏族|974,青海共和|975,青海玛沁(果洛)|976,青海玉树|977,青海德令哈|979,青海格尔木"\
	"|990,新疆克拉玛依|991,新疆乌鲁木齐|992,新疆奎屯|993,新疆石河子|994,新疆昌吉|995,新疆吐鲁番|996,新疆库尔勒|997,新疆阿克苏|998,新疆喀什|999,新疆伊犁"\
	"|8017,西藏尼木|8020,西藏贡嘎|8051,西藏江达|8054,西藏芒康|8056,西藏八宿|8057,西藏洛龙|8059,西藏丁青|8061,西藏巴青|8062,西藏比如|8067,西藏班戈|8069,西藏措勤|8078,西藏索县|8081,西藏尼玛|"
};

const char _mobile_providers[] = {
	"|134,移动|135,移动|136,移动|137,移动|138,移动|139,移动|150,移动|151,移动|152,移动|157,移动|158,移动|159移动" \
	"|182,移动|187,移动|188,移动" \
	"|130,联通|131,联通|132,联通|155,联通|156,联通" \
	"|185,联通|186,联通" \
	"|133,电信|153,电信" \
	"|180,电信|189,电信|"
};

#endif /* __AREACODE_H */