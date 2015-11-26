#include <conio.h>
#include "TVoctreeVLFeat.h"
#include <omp.h>
#include "iostream"
#include "math.h"
#include <string>
#include "TFeatureExtractor.h"


using namespace std;

//argv[1] = input type
		// ** 1 = read all images from one folder
		// ** 2 = read all images from year folder (includes month 
		//subfolders and these folders also includes day subfolders)
//argv[2] = input folder address
//argv[3] = Tree Type
//argv[4] = SIFT name
//argv[5] = 1 for RootSIFT 0 othervise
//argv[6] = Descriptor Name
		
enum Root_Mode { Root_SIFT = 1, SIFT};

const char * VL_FeatTree_Names [] = {"","Small_Tree", "Middle_Tree", "Large_Tree", "TEST_Tree"};
const char * Root_Names [] = {"__", "RootSIFT"};

int main(int argc, const char * argv[]) 
{
	string flickerPath = "D:/Data/flicker100K";
	string dscName = "dsc_akaze2";
	vl_size numdesc = 90000000;//9M for 10k images 90M for 100K
	unsigned char* data = new unsigned char[numdesc*61];
	vl_size DataSize = 0;
	read_DSC_from_flicker1M(flickerPath, dscName, data, DataSize,numdesc);

	TVoctreeVLFeat VT;	
	VT.m_stupidCount = 0;	
	VT.init(data,DataSize,VLFeat_Init_Mode::Small_Tree, 61);

	string writeAddr =  (string)"D:/Data/VT_Trees/VT_flicker10K_AKAZE_feats_small_tree_P.dat";

	VT.write_hikm(writeAddr.c_str());	
	printf("Tree is written!\n");
	printf("read!\n");

	//

	int MaxNode = VT.ReturnMaxNodeNumber();

	printf("Num nodes created: %d\n", VT.m_stupidCount);
	if(MaxNode>VT.m_stupidCount)
	{
		printf("Num of max nodes: %d\n", MaxNode);
		printf("Empty branches exist! It can cause wrong quantization\n");		
	}	
	return 0;
}
//
//int main2 (int argc, const char * argv[]) 
//{
//	// bu 1 kere burada yapilacak baska yere goturme
//	int x = omp_get_num_procs(); 
//	omp_set_num_threads(x);		   		
//	 
//	
//	string FolderAddr ;
//	
//	vl_size numdesc = 100000;//For 80M desc
//	unsigned char* data = new unsigned char[numdesc*128];
//	vl_size DataSize = 0;
//	if(argc>0)
//	{
//		FolderAddr = argv[2];
//		int FolderReadFormat = atoi(argv[1]);
//		if(argv[3])
//		switch (FolderReadFormat)
//		{
//			case 1:
//				read_SIG_V2s_From_OneFolder(FolderAddr, data, DataSize, numdesc, atoi(argv[5]));break;
//			case 2:
//				read_SIGs_From_YearFolder(FolderAddr, data, DataSize, numdesc, atoi(argv[5]));break;
//			default:
//				read_SIG_V2s_From_OneFolder(FolderAddr, data, DataSize, numdesc,0);break;
//
//		}
//	}
//	else
//	{
//		FolderAddr = "D:/ImageSearchEngine/Dumps/100MSig2s_SIFT_HELL_ZSTD";
//		read_SIG_V2s_From_OneFolder(FolderAddr,data,DataSize, numdesc,atoi(argv[5]));
//	}
//	TVoctreeVLFeat VT;	
//	VT.m_stupidCount = 0;	
//	VT.init(data,DataSize,atoi(argv[3]), 128);
//
//	if(argc<7)
//		argv[6] = "__";
//	
//	string writeAddr =  (string)"D:/Data/VT_"+ VL_FeatTree_Names[atoi(argv[3])] + "_"
//	+ argv[4]+ "_" +argv[6]	+"_" + Root_Names[atoi(argv[5])] + ".dat";
//	
//	VT.write_hikm(writeAddr.c_str());	
//	printf("Tree is written!\n");
//	printf("read!\n");
//
//	//
//
//	int MaxNode = VT.ReturnMaxNodeNumber();
//
//	printf("Num nodes created: %d\n", VT.m_stupidCount);
//	if(MaxNode>VT.m_stupidCount)
//	{
//		printf("Num of max nodes: %d\n", MaxNode);
//		printf("Empty branches exist! It can cause wrong quantization\n");		
//	}	
//	
//	//VT.quantize_multi(vwi,testData,testDataSize);	
//
//
//
//
//	///////////////////TEST PART //////////////////////
//
//	//TVoctreeVLFeat otherVT;
//	//printf("Inited!\n");
//	//otherVT.init_read("D:/Data/VT_Middle_Tree_100MSig2s_SIFT_HELL_ZSTD_SIFT.dat");
//	
//	
//	
//	/*double qErr = 0;
//	for (int i=0; i<testDataSize; i++)
//		qErr += (double)abs((double)(vwi[i]-vwi2[i]));
//	printf("Error: %f\n", qErr);*/
//
//	///Creating TestData from Center Points////
//	//int testDataSize = 10000;
//	//unsigned int *vwi = new unsigned int[testDataSize];
//	//vl_ikmacc_t* testData = new vl_ikmacc_t[testDataSize*128];
//	//int counter = 0;
//
//	//
//	//
//	//for(int tiiiii = 0 ; tiiiii < 10 ; tiiiii++){
//	//for(int tiiii = 0 ; tiiii < 10 ; tiiii++){
//	//for(int tiii = 0 ; tiii < 10 ; tiii++){
//	//for(int tii = 0 ; tii < 10 ; tii++){
//	//	for(int ti=0; ti<10; ti++)
//	//	{
//	//		vl_ikmacc_t* center = ((((((((((((((otherVT).vocabTree).root)->children[tiiiii]))->children[tiiii]))->children[tiii]))->children[tii]))->children[ti]))->filter)->centers;
//	//		memcpy(&testData[(counter)*1280],center,128*4*10);
//	//		for(int ri=0; ri<10; ri++)
//	//		{
//	//			
//	//	/*		testData[(counter)*1280+(ri*128+5)] = testData[(counter)*1280+(ri*128+5)] + (rand() % 10);
//	//			testData[(counter)*1280+(ri*128+10)] = testData[(counter)*1280+(ri*128+10)] - (rand() % 10);
//	//			testData[(counter)*1280+(ri*128+40)] = testData[(counter)*1280+(ri*128+40)] + (rand() % 15);
//	//			testData[(counter)*1280+(ri*128+80)] = testData[(counter)*1280+(ri*128+80)] - (rand() % 15);
//	//			testData[(counter)*1280+(ri*128+100)] = testData[(counter)*1280+(ri*128+100)] + (rand() % 5);*/
//	//		
//	//		}
//	//		counter++;
//	//	}
//	//}}}}
//	//
//	//unsigned char* testDataR = new unsigned char[testDataSize*128];
//	//for (int ii=0; ii<testDataSize*128; ii++)
//	//{
//	//	testDataR[ii] = (unsigned char)testData[ii];
//	//}
//
//
//	//char fileName[1000] = "D:\\ImageSearchEngine\\Dumps\\100MSig2s_SIFT_HELL_ZSTD\\00000F94-A0DD-44ED-A15B-89AB227E53DA.sig";
//	
//	//		const char* sigFile = "D:\\ImageSearchEngine\\Dumps\\100MSig2s_SIFT_HELL_ZSTD\\00000F94-A0DD-44ED-A15B-89AB227E53DA.sig";
//	//		//const char* sigFile = "D:/ImageSearch/5004.sig";
//
//	//		TSignature mySig ;
//	//		unsigned char * RootSIFTptr;
//	//		unsigned char * MySiftu;
//	//		if(is_sig_file(sigFile))
//	//		{
//	//			
//	//			mySig = t_feat_read_sig_v2(sigFile);
//	//			
//	//			unsigned int numdesc = mySig.numKeypts;
//	//				//fread(&numdesc, sizeof(unsigned int), 1, f);
//	//			RootSIFTptr = new unsigned char[numdesc*128];
//	//			MySiftu = new unsigned char[numdesc*128];
//	//			memcpy(RootSIFTptr, mySig.keyPoints,(numdesc)*128*sizeof(unsigned char));
//	//			memcpy(MySiftu, mySig.keyPoints,(numdesc)*128*sizeof(unsigned char));
//
//	//			for(int ri = 0;  ri<numdesc*128; ri++)
//	//			{
//	//				RootSIFTptr[ri] = sqrt(RootSIFTptr[ri]);
//	//			}
//	//			//memcpy(MySiftu, RootSIFTptr, (numdesc)*128);
//	//			
//	//					
//	//				
//	//			
//	//		}
//	/////////////////////////////////////////////////////
//
//	//printf("quantizing!\n");
//	//unsigned int *vwi = new unsigned int[mySig.numKeypts]();
//	//otherVT.quantize_multi(vwi,RootSIFTptr,mySig.numKeypts);
//
//
//	//unsigned int *vwi2 = new unsigned int[mySig.numKeypts]();
//	//otherVT.quantize_multi(vwi2,RootSIFTptr,mySig.numKeypts);
//
//
//
//////////
//	////for (int qi=0; qi<1000000; qi++)
//	////	otherVT.quantize(&vwi[qi],&testDataR[qi*128]);
//	//vector<long long> errList,errListRes;
//	//double qErr = 0;
//	//for (unsigned long long i=0; i<testDataSize; i++)
//	//{
//
//	//	qErr += (double)abs((double)(vwi[i]-i));
//	//	if(vwi[i]-i)
//	//	{
//	//		errList.push_back(i);
//	//		errListRes.push_back(vwi[i]);
//	//	}
//	//}
//
//	//for (int qi=0; qi<errList.size(); qi++)
//	//	otherVT.quantize(&vwi[errList[qi]],&testDataR[errList[qi]*128]);
//
//	//printf("Error: %d\n", errList.size());
//	
//	VL_PRINT ("VL HIKMEANS IS DONE!\n") ;
//  return 0;
//}