namespace Nektar
{
    namespace LibUtilities 
    {
        static const unsigned int perm3A_2d[3][3] = {{0,1,2},{2,0,1},{0,2,1}}; // Works for aab
        static const unsigned int perm3B_2d[3][3] = {{0,1,2},{1,0,2},{1,2,0}}; // Works for abb
        static const unsigned int perm3C_2d[3][3] = {{0,1,2},{2,0,1},{1,2,0}}; // Works for aab and abb
        static const unsigned int perm6_2d [6][3] = {{0,1,2},{1,0,2},{2,0,1},
                                                     {2,1,0},{0,2,1},{1,2,0}}; // Works for abc
        const unsigned int NodalTriFeketeAvailable = 16;
        static const unsigned int NodalTriFeketeNPTS[NodalTriFeketeAvailable] = {1,2,3,4,5,7,8,10,12,14,16,19,21,24,27,30};
        static const double NodalTriFeketeData[][6] = {
     // %%% n_1   n_3   n_6          l_1            l_2           l_3
            // 1 1 %%% Order / Number of Points
            {0,    2,    0,     1.0000000000,  0.0000000000,  0.0000000000},
            // 2 2 %%% Order / Number of Points
            {0,    2,    0,     1.0000000000,  0.0000000000,  0.0000000000},
            {0,    1,    0,     0.5000000000,  0.5000000000,  0.0000000000},
            // 3 3 %%% Order / Number of Points
            {0,    2,    0,     1.0000000000,  0.0000000000,  0.0000000000},
            {0,    0,    1,     0.7236067977,  0.2763932023,  0.0000000000},
            {1,    0,    0,     0.3333333333,  0.3333333333,  0.3333333333},
            // 4 4 %%% Order / Number of Points
            {0,    2,    0,     1.0000000000,  0.0000000000,  0.0000000000},
            {0,    1,    0,     0.5000000000,  0.5000000000,  0.0000000000},
            {0,    0,    1,     0.8273268354,  0.1726731646,  0.0000000000},
            {0,    2,    0,     0.5669152707,  0.2165423647,  0.2165423647},
            // 5 5 %%% Order / Number of Points
            {0,    2,    0,     1.0000000000,  0.0000000000,  0.0000000000},
            {0,    0,    1,     0.8825276620,  0.1174723380,  0.0000000000},
            {0,    0,    1,     0.6426157582,  0.3573842418,  0.0000000000},
            {0,    2,    0,     0.7039610574,  0.1480194713,  0.1480194713},
            {0,    1,    0,     0.4208255393,  0.4208255393,  0.1583489214},
            // 6 7 %%% Order / Number of Points
            {0,     2,   0,     1.0000000000,  0.0000000000,  0.0000000000},
            {0,     1,   0,     0.5000000000,  0.5000000000,  0.0000000000},
            {0,     0,   1,     0.9151145777,  0.0848854223,  0.0000000000},
            {0,     0,   1,     0.7344348598,  0.2655651402,  0.0000000000},
            {1,     0,   0,     0.3333333333,  0.3333333333,  0.3333333333},
            {0,     2,   0,     0.7873290632,  0.1063354684,  0.1063354684},
            {0,     0,   1,     0.5665492870,  0.3162697959,  0.1171809171},
            // 7 8 %%% Order / Number of Points
            {0,     2,   0,     1.0000000000,  0.0000000000,  0.0000000000},
            {0,     0,   1,     0.9358700743,  0.0641299257,  0.0000000000},
            {0,     0,   1,     0.7958500908,  0.2041499092,  0.0000000000},
            {0,     0,   1,     0.6046496090,  0.3953503910,  0.0000000000},
            {0,     2,   0,     0.8408229862,  0.0795885069,  0.079588506},
            {0,     2,   0,     0.4735817901,  0.2632091050,  0.2632091050},
            {0,     1,   0,     0.4539853441,  0.4539853441,  0.0920293119},
            {0,     0,   1,     0.6676923094,  0.2433646431,  0.0889430475},
            // 8 10 %%% Order / Number of Points
            {0,     2,   0,     1.0000000000,  0.0000000000,  0.0000000000},
            {0,     1,   0,     0.5000000000,  0.5000000000,  0.0000000000},
            {0,     0,   1,     0.9498789977,  0.0501210023,  0.0000000000},
            {0,     0,   1,     0.8385931398,  0.1614068602,  0.0000000000},
            {0,     0,   1,     0.6815587319,  0.3184412681,  0.0000000000},
            {0,     2,   0,     0.8444999609,  0.0777500195,  0.0777500195},
            {0,     1,   0,     0.4683305115,  0.4683305115,  0.0633389771},
            {0,     1,   0,     0.3853668203,  0.3853668204,  0.2292663593},
            {0,     0,   1,     0.7172965409,  0.2335581033,  0.0491453558},
            {0,     0,   1,     0.5853134902,  0.2667701010,  0.1479164088},
            // 9 12 %%% Order / Number of Points
            {0,     2,   0,     1.0000000000,  0.0000000000,  0.0000000000},
            {0,     0,   1,     0.9597669930,  0.0402330070,  0.0000000000},  
            {0,     0,   1,     0.8693870908,  0.1306129092,  0.0000000000},  
            {0,     0,   1,     0.7389628040,  0.2610371960,  0.0000000000},  
            {0,     0,   1,     0.5826397065,  0.4173602935,  0.0000000000},  
            {1,     0,   0,     0.3333333333,  0.3333333333,  0.3333333333}, 
            {0,     2,   0,     0.9021308608,  0.0489345696,  0.0489345696}, 
            {0,     2,   0,     0.6591363598,  0.1704318201,  0.1704318201}, 
            {0,     1,   0,     0.4699587644,  0.4699587644,  0.0600824712}, 
            {0,     0,   1,     0.7904339977,  0.1543901944,  0.0551758079},  
            {0,     0,   1,     0.6401193011,  0.3010242110,  0.0588564879}, 
            {0,     0,   1,     0.4963227512,  0.3252434900,  0.1784337588}, 
            // 10 14 %%% Order / Number of Points                                              
            {0,     2,   0,    1.0000000000,  0.0000000000,  0.0000000000},    
            {0,     2,   0,    0.5000000000,  0.5000000000,  0.0000000000},    
            {0,     0,   1,    0.9670007152,  0.0329992848,  0.0000000000},   
            {0,     0,   1,    0.8922417368,  0.1077582632,  0.0000000000},    
            {0,     0,   1,    0.7826176635,  0.2173823365,  0.0000000000},    
            {0,     0,   1,    0.6478790678,  0.3521209322,  0.0000000000},    
            {0,     2,   0,    0.9145236987,  0.0427381507,  0.0427381507},    
            {0,     2,   0,    0.5331019411,  0.2334490294,  0.2334490294},    
            {0,     1,   0,    0.4814795342,  0.4814795342,  0.0370409316},    
            {0,     1,   0,    0.3800851251,  0.3800851251,  0.2398297498},    
            {0,     0,   1,    0.8150971991,  0.1351329831,  0.0497698178},    
            {0,     0,   1,    0.6778669104,  0.2844305545,  0.0377025351},    
            {0,     0,   1,    0.6759450113,  0.2079572403,  0.1160977485},    
            {0,     0,   1,    0.5222323306,  0.3633472465,  0.1144204229},    
            // 11 16 %%% Order / Number of Points
            {0,     2,   0,     1.0000000000,  0.0000000000,  0.0000000000},    
            {0,     0,   1,     0.9724496361,  0.0275503639,  0.0000000000},    
            {0,     0,   1,     0.9096396608,  0.0903603392,  0.0000000000},    
            {0,     0,   1,     0.8164380765,  0.1835619235,  0.0000000000},    
            {0,     0,   1,     0.6997654705,  0.3002345295,  0.0000000000},    
            {0,     0,   1,     0.5682764664,  0.4317235336,  0.0000000000},    
            {0,     2,   0,     0.9201760661,  0.0399119670,  0.0399119670},    
            {0,     2,   0,     0.8097416696,  0.0951291652,  0.0951291652},    
            {0,     2,   0,     0.4216558161,  0.2891720920,  0.2891720920},    
            {0,     1,   0,     0.4200100315,  0.4200100315,  0.1599799371},    
            {0,     1,   0,     0.4832770031,  0.4832770031,  0.0334459938},    
            {0,     0,   1,     0.8236881237,  0.1452587341,  0.0310531421},    
            {0,     0,   1,     0.7030268141,  0.2021386640,  0.0948345219},    
            {0,     0,   1,     0.6642752329,  0.3066778199,  0.0290469472},    
            {0,     0,   1,     0.5605605456,  0.3510551601,  0.0883842943},
            {0,     0,   1,     0.5584153138,  0.2661283688,  0.1754563174},
            // 12 19 %%% Order / Number of Points
            {0,     2,   0,     1.0000000000,  0.0000000000,  0.0000000000},  
            {0,     1,   0,     0.5000000000,  0.5000000000,  0.0000000000},  
            {0,     0,   1,     0.9766549233,  0.0233450767,  0.0000000000},  
            {0,     0,   1,     0.9231737823,  0.0768262177,  0.0000000000},  
            {0,     0,   1,     0.8430942345,  0.1569057655,  0.0000000000},  
            {0,     0,   1,     0.7414549105,  0.2585450895,  0.0000000000},  
            {0,     0,   1,     0.6246434651,  0.3753565349,  0.0000000000},  
            {1,     0,   0,     0.3333333333,  0.3333333333,  0.3333333333}, 
            {0,     1,   0,     0.4005558261,  0.4005558262,  0.1988883477}, 
            {0,     2,   0,     0.4763189598,  0.2618405201,  0.2618405201}, 
            {0,     2,   0,     0.8385226450,  0.0807386775,  0.0807386775}, 
            {0,     2,   0,     0.9326048528,  0.0336975736,  0.0336975736}, 
            {0,     0,   1,     0.8530528428,  0.1206826354,  0.0262645218}, 
            {0,     0,   1,     0.7260597695,  0.2489279690,  0.0250122615}, 
            {0,     0,   1,     0.6376427136,  0.2874821712,  0.0748751152}, 
            {0,     0,   1,     0.5625832895,  0.4071849276,  0.0302317829}, 
            {0,     0,   1,     0.7415828366,  0.1697134458,  0.0887037176}, 
            {0,     0,   1,     0.5954847541,  0.2454317980,  0.1590834479}, 
            {0,     0,   1,     0.5072511952,  0.3837518758,  0.1089969290},  
            // 13 21 %%% Order / Number of Points                                              
            {0,     2,   0,    1.0000000000,  0.0000000000,  0.0000000000},   
            {0,     0,   1,    0.9799675226,  0.0200324774,  0.0000000000},   
            {0,     0,   1,    0.9339005269,  0.0660994731,  0.0000000000},   
            {0,     0,   1,    0.8644342995,  0.1355657005,  0.0000000000},   
            {0,     0,   1,    0.7753197015,  0.2246802985,  0.0000000000},   
            {0,     0,   1,    0.6713620067,  0.3286379933,  0.0000000000},   
            {0,     0,   1,    0.5581659344,  0.4418340656,  0.0000000000},   
            {0,     2,   0,    0.8520024555,  0.0739987723,  0.0739987723},   
            {0,     2,   0,    0.7432264994,  0.1283867503,  0.1283867503},   
            {0,     2,   0,    0.5395738831,  0.2302130584,  0.2302130584},   
            {0,     1,   0,    0.4644144021,  0.4644144021,  0.0711711958},   
            {0,     2,   0,    0.4114075139,  0.2942962431,  0.2942962431},   
            {0,     1,   0,    0.3929739081,  0.3929739081,  0.2140521839},   
            {0,     0,   1,    0.9198937820,  0.0580254800,  0.0220807380},   
            {0,     0,   1,    0.8269495692,  0.1490288338,  0.0240215969},   
            {0,     0,   1,    0.7425432195,  0.1884717952,  0.0689849852},   
            {0,     0,   1,    0.7075668818,  0.2676625888,  0.0247705295},   
            {0,     0,   1,    0.6366434033,  0.2298513909,  0.1335052058},   
            {0,     0,   1,    0.5976134593,  0.3320684992,  0.0703180415},   
            {0,     0,   1,    0.5665760284,  0.4108329044,  0.0225910673},   
            {0,     0,   1,    0.5028853010,  0.3560657231,  0.1410489759},   
            // 14 24 %%% Order / Number of Points
            {0,     2,   0,    1.0000000000,  0.0000000000,  0.0000000000},
            {0,     1,   0,    0.5000000000,  0.5000000000,  0.0000000000},   
            {0,     0,   1,    0.9826229633,  0.0173770367,  0.0000000000},   
            {0,     0,   1,    0.9425410221,  0.0574589779,  0.0000000000},   
            {0,     0,   1,    0.8817598450,  0.1182401550,  0.0000000000},   
            {0,     0,   1,    0.8031266027,  0.1968733973,  0.0000000000},   
            {0,     0,   1,    0.7103190274,  0.2896809726,  0.0000000000},   
            {0,     0,   1,    0.6076769777,  0.3923230223,  0.0000000000},       
            {0,     2,   0,    0.9545282960,  0.0227358520,  0.0227358520},   
            {0,     2,   0,    0.7279988378,  0.1360005811,  0.1360005811},   
            {0,     2,   0,    0.6146143335,  0.1926928332,  0.1926928332},   
            {0,     1,   0,    0.4704320969,  0.4704320969,  0.0591358062},   
            {0,     2,   0,    0.4062455674,  0.2968772163,  0.2968772163},   
            {0,     1,   0,    0.3997167456,  0.3997167456,  0.2005665089},   
            {0,     0,   1,    0.8994312852,  0.0727092891,  0.0278594257},   
            {0,     0,   1,    0.8235331194,  0.1556596225,  0.0208072581},   
            {0,     0,   1,    0.8159593721,  0.1175577052,  0.0664829227},   
            {0,     0,   1,    0.7277004755,  0.2121104191,  0.0601891053},   
            {0,     0,   1,    0.7017966273,  0.2792485214,  0.0189548513},   
            {0,     0,   1,    0.6286342673,  0.2519081185,  0.1194576142},  
            {0,     0,   1,    0.6152236423,  0.3254238863,  0.0593524714},   
            {0,     0,   1,    0.5599300851,  0.4197058520,  0.0203640629},
            {0,     0,   1,    0.5124435882,  0.2931807429,  0.1943756690},
            {0,     0,   1,    0.5038948451,  0.3812965190,  0.1148086359},
            // 15 27 %%% Order / Number of Points
            {0,     2,   0,     1.0000000000,  0.0000000000,  0.0000000000},  
            {0,     0,   1,     0.9847840231,  0.0152159769,  0.0000000000},  
            {0,     0,   1,     0.9496002665,  0.0503997335,  0.0000000000},  
            {0,     0,   1,     0.8960041459,  0.1039958541,  0.0000000000},  
            {0,     0,   1,     0.8261943514,  0.1738056486,  0.0000000000},  
            {0,     0,   1,     0.7430297109,  0.2569702891,  0.0000000000},  
            {0,     0,   1,     0.6499152345,  0.3500847655,  0.0000000000},  
            {0,     0,   1,     0.5506631368,  0.4493368632,  0.0000000000},  
            {1,     0,   0,     0.3333333333,  0.3333333333,  0.3333333333}, 
            {0,     2,   0,     0.9561008318,  0.0219495841,  0.0219495841},       
            {0,     2,   0,     0.8937652924,  0.0531173538,  0.0531173538},     
            {0,     2,   0,     0.7626684778,  0.1186657611,  0.1186657611},      
            {0,     2,   0,     0.5478917292,  0.2260541354,  0.2260541354},   
            {0,     1,   0,     0.4761452138,  0.4761452137,  0.0477095725},    
            {0,     1,   0,     0.4206960976,  0.4206960976,  0.1586078048}, 
            {0,     0,   1,     0.9011419064,  0.0816639421,  0.0171941515},     
            {0,     0,   1,     0.8302595715,  0.1162464503,  0.0534939782},    
            {0,     0,   1,     0.8059790318,  0.1759511193,  0.0180698489},   
            {0,     0,   1,     0.7424917767,  0.1978887556,  0.0596194677}, 
            {0,     0,   1,     0.6958779924,  0.2844332752,  0.0196887324},      
            {0,     0,   1,     0.6684861226,  0.1975591066,  0.1339547708},     
            {0,     0,   1,     0.6374939126,  0.2749910734,  0.0875150140},      
            {0,     0,   1,     0.6000365168,  0.3524012205,  0.0475622627},       
            {0,     0,   1,     0.5666808380,  0.4176001732,  0.0157189888},    
            {0,     0,   1,     0.5400834895,  0.3013819154,  0.1585345951},    
            {0,     0,   1,     0.5173966708,  0.3853507643,  0.0972525649},    
            {0,     0,   1,     0.4350225702,  0.3270403780,  0.2379370518},  
            // 16 30 %%% Order / Number of Points
            {0,     2,   0,    1.0000000000,  0.0000000000,  0.0000000000},  
            {0,     1,   0,    0.5000000000,  0.5000000000,  0.0000000000},   
            {0,     0,   1,    0.9865660883,  0.0134339117,  0.0000000000},   
            {0,     0,   1,    0.9554399980,  0.0445600020,  0.0000000000},   
            {0,     0,   1,    0.9078481256,  0.0921518744,  0.0000000000},   
            {0,     0,   1,    0.8455144903,  0.1544855097,  0.0000000000},   
            {0,     0,   1,    0.7706926997,  0.2293073003,  0.0000000000},   
            {0,     0,   1,    0.6860872168,  0.3139127832,  0.0000000000},   
            {0,     0,   1,    0.5947559868,  0.4052440132,  0.0000000000},   
            {0,     2,   0,    0.9082733123,  0.0458633439,  0.0458633439},   
            {0,     2,   0,    0.7814558654,  0.1092720673,  0.1092720673},   
            {0,     2,   0,    0.5061583246,  0.2469208377,  0.2469208377},   
            {0,     1,   0,    0.4752124798,  0.4752124798,  0.0495750405},   
            {0,     1,   0,    0.4264731058,  0.4264731058,  0.1470537884},   
            {0,     2,   0,    0.3970899287,  0.3014550356,  0.3014550356},   
            {0,     1,   0,    0.3866478326,  0.3866478326,  0.2267043349},   
            {0,     0,   1,    0.9485194031,  0.0373109962,  0.0141696007},   
            {0,     0,   1,    0.8743842850,  0.1097814309,  0.0158342841},   
            {0,     0,   1,    0.8512484463,  0.0959614844,  0.0527900693},   
            {0,     0,   1,    0.7826094027,  0.1639276476,  0.0534629497},   
            {0,     0,   1,    0.7790251588,  0.2050142286,  0.0159606126},   
            {0,     0,   1,    0.7045940167,  0.2476638915,  0.0477420917},   
            {0,     0,   1,    0.7008539071,  0.1815134068,  0.1176326861},   
            {0,     0,   1,    0.6644617368,  0.3197919643,  0.0157462988},   
            {0,     0,   1,    0.6229177644,  0.2778907781,  0.0991914575},   
            {0,     0,   1,    0.6010014909,  0.3493750783,  0.0496234308},   
            {0,     0,   1,    0.6009076040,  0.2332142592,  0.1658781368},   
            {0,     0,   1,    0.5481915845,  0.4347608890,  0.0170475265}, 
            {0,     0,   1,    0.5160516828,  0.3894384191,  0.0945098981}, 
            {0,     0,   1,    0.5043443688,  0.3260983223,  0.1695573088}};
    }
}
