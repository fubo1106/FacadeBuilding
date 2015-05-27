#include "Segmentation.h"
#include "Basics.h"

#include <assert.h>

extern QtAPI QtApi;

Segmentation::Segmentation(){
}

Segmentation::Segmentation(cv::Mat& src, cv::Mat& dst)
{
	_src = src.clone();
	_dst = dst.clone(); 
}

Segmentation::Segmentation(cv::Mat& src,string path)
{
	_src = src.clone();
	_imagePath = path;
	int r = path.find_last_of('.');
	
	int l = path.find_last_of('/');
	if (l == path.npos)
		l = path.find_last_of('\\');
	_baseName = path.substr(l + 1, r - l - 1);
	calcAttribute();
}

Segmentation::~Segmentation()
{
	if (_rgradient != NULL)
		delete _rgradient;
	if (_cgradient != NULL)
		delete _cgradient;
}

void Segmentation::calcAttribute(){
	Mat gradSrc;
	//Mat visual = Mat::zeros(_src.rows, _src.cols, CV_8UC1);

	if (_src.channels() == 3)
		cvtColor(_src, gradSrc, CV_BGR2GRAY);

	//each col gradient 
	_cgradient = new double[_src.cols]; //record sum gray for each colume

	for (int i = 0; i < _src.cols; i++){
		_cgradient[i] = 0;
	}

	for (int j = 0; j < _src.cols; j++){
		for (int i = 0; i < _src.rows; i++)
			_cgradient[j] += _src.at<uchar>(i, j);
		_cgradient[j] /= _src.rows;
		if (j > 0){
			_cgradient[j - 1] = abs(_cgradient[j] - _cgradient[j - 1]);
		}
	}

	//each row gradient 
	_rgradient = new double[_src.rows]; //record sum gray for each row

	for (int i = 0; i < _src.rows; i++){
		_rgradient[i] = 0;
	}

	for (int i = 0; i < _src.rows; i++){
		for (int j = 0; j < _src.cols; j++)
			_rgradient[i] += _src.at<uchar>(i, j);
		_rgradient[i] /= _src.cols;
		if (i > 0){
			_rgradient[i - 1] = abs(_rgradient[i] - _rgradient[i - 1]);
		}
	}

	//test gradient
	/*Mat visual = Mat::zeros(_src.rows, _src.cols, CV_8UC1);
	for (int j = 0; j < _src.cols; j++){
		for (int i = 0; i < _src.rows; i++){
			visual.at<uchar>(i, j) = _cgradient[j];
		}
	}
	imshow("visual", visual);
	waitKey(0);*/

	//init _baseName


}

void Segmentation::ImageSegmentation(Mat& src, Mat& res){
	//Canny(src, res, 50, 100); 
	Mat gradSrc;
	Mat visual = Mat::zeros(src.rows, src.cols, CV_8UC1);

	cvtColor(src, gradSrc, CV_BGR2GRAY);
	double* avggray;
	avggray = new double[src.cols]; //record sum gray for each colume

	vector<double> scale;
	scale.resize(src.rows);//record gray scale for rows

	for (int i = 0; i < src.cols; i++){
		avggray[i] = 0;
	}


	for (int j = 0; j < src.cols; j++){
		for (int i = 0; i < src.rows; i++)
			avggray[j] += src.at<uchar>(i, j);
		avggray[j] /= src.rows;
		if (j >= 2){
			double del1 = abs(avggray[j] - avggray[j - 1]);
			double del2 = abs(avggray[j-1] - avggray[j - 2]);
			if (del1 >= 10*del2)
				avggray[j - 1] = -1;//record segment line
		}
	}
	
	for (int j = 0; j < visual.cols; j++){
		if (avggray[j] == -1)
			line(src, Point(j, 0), Point(j, visual.rows - 1), Scalar(0, 0, 255), 2, CV_AA);
	}

	for (int j = 0; j < visual.cols; j++){
		for (int i = 0; i < visual.rows; i++){
			visual.at<uchar>(i, j) = avggray[j];
		}
	}
	//imshow("visualize", visual);
	res = visual;
	//waitKey(0);
	delete avggray;
	return;
}

int Segmentation::getBestSegAxis(char whichaxis, int userselected){
	/*coordinate: ------------->  x axis
				  |
			y axis|
				  |
	*/
	int l, r;
	if (whichaxis == 'x'){
		/*horizen axis: -------->*/
		for (int i = userselected; i < _src.rows; i++){
			if (i >= 2){
				double del1 = abs(_rgradient[i] - _rgradient[i - 1]);
				double del2 = abs(_rgradient[i - 1] - _rgradient[i - 2]);
				if (del1 >= 10 * del2){
					r = i;//record segment line
					break;
				}
			}
		}
		for (int i = userselected; i > 0; i--){
			if (i >= 2){
				double del1 = abs(_rgradient[i] - _rgradient[i - 1]);
				double del2 = abs(_rgradient[i - 1] - _rgradient[i - 2]);
				if (del2 >= 10 * del1){
					l = i;//record segment line
					break;
				}
			}
		}
	}
	else{/*vertical axis --------*/
		for (int i = userselected; i < _src.cols; i++){
			if (i >= 2){
				double del1 = abs(_cgradient[i] - _cgradient[i - 1]);
				double del2 = abs(_cgradient[i - 1] - _cgradient[i - 2]);
				if (del1 >= 10 * del2){
					r = i;//record segment line
					break;
				}
					
			}
		}
		for (int i = userselected; i > 0; i--){
			if (i >= 2){
				double del1 = abs(_cgradient[i] - _cgradient[i - 1]);
				double del2 = abs(_cgradient[i - 1] - _cgradient[i - 2]);
				if (del2 >= 10 * del1){
					l = i;//record segment line
					break;
				}
			}
		}
	}

	/*if (abs((l + r) / 2 - userselected) < 50)
		return (l + r) / 2;
	else*/
		return userselected;
}

bool smallerThan(int p1, int p2){
	return p1 < p2;
}

void Segmentation::saveSegments(Mat& src, vector<pair<char, int>>& axis, string dir){
	/*coordinate: ------------->  x axis
			  |
			  |
	 		  |  y axis
	 x:  horizen axis: -------->
	 y:  vertical axis
	*/
	Mat imageroi;

	vector<int> ax, ay;
	for (int i = 0; i < axis.size(); i++){
		if (axis[i].first == 'x')
			ax.push_back(axis[i].second);
		else
			ay.push_back(axis[i].second);
	}
	sort(ax,smallerThan); 
	sort(ay,smallerThan);

	for (int i = 0; i < ax.size()-1; i++){
		for (int j = 0; j < ay.size()-1; j++){
			Rect rect(ay[j], ax[i], ay[j + 1] - ay[j], ax[i + 1] - ax[i]);
			imageroi = src(rect);
			//imshow("roi", imageroi); waitKey(0);
			if (!boost::filesystem::exists(dir +"\\"+_baseName))
				boost::filesystem::create_directories(dir +"\\"+ _baseName);
			imwrite(dir+"\\"+_baseName+"\\part-[" + boost::lexical_cast<string>(i)+"," + boost::lexical_cast<string>(j)+"].jpg", imageroi);
		}
	}

	return;
}

/*kmeans_seg each jpg images*/
void Segmentation::kmeans_seg_folder(string srcDir,string dstDir){
	int nclusters = 5;

	if (!boost::filesystem::exists(srcDir)){
		cout << "src directory not exists";
		return;
	}

	fs::path fullpath(srcDir, fs::native);
	if (!fs::exists(fullpath)) return;

	fs::recursive_directory_iterator end_iter;
	for (fs::recursive_directory_iterator iter(fullpath); iter != end_iter; iter++)
	{
		if (!fs::is_directory(*iter)){
			//cout << "current image" << iter->path().string() << endl;
			string currentImagePath = iter->path().string();
			string currentImageS = iter->path().filename().string();
			_baseName = currentImageS.substr(0, currentImageS.find_first_of('.'));
			string ext = currentImageS.substr(currentImageS.find_first_of('.')+1, currentImageS.length()-1);
			if (ext == "jpg"){
				Mat img = imread(currentImagePath, 1);
				_src = img;
				Mat result, centers, visual;
				if (boost::filesystem::exists(dstDir + _baseName + "\\labels.txt") &&
					boost::filesystem::exists(dstDir + _baseName + "\\centers.txt") &&
					boost::filesystem::exists(dstDir + _baseName + "\\scolor.txt") &&
					boost::filesystem::exists(dstDir + _baseName + "\\label_weight.txt")){
					QtApi.sendMsg(currentImagePath + " already been saved.. continue..\n");
					continue;//already exist 
				}	
				else{
					kmeans_seg(img, result, centers, visual, nclusters);
					save_kmeans(dstDir);
				}		
			}
			else continue;
			
		}
	}
}

void Segmentation::kmeans_seg(Mat& src, Mat& result, Mat& centers, Mat& visual, int nclusters){

	assert(src.channels() == 3); 
	assert(src.rows > 0);

	result = Mat(src.rows, src.cols, CV_8UC1);
	visual = Mat(src.rows, src.cols, CV_8UC3);
	RNG rng(12345); //随机数产生器

	Scalar colorTab[] =     //最多只有10类，所以最多也就给10个颜色
	{
		Scalar(DEPTH_0, DEPTH_0, DEPTH_0), //blue #696969 105, 105, 105
		Scalar(DEPTH_1, DEPTH_1, DEPTH_1), //blue #696969 105, 105, 105
		Scalar(DEPTH_2, DEPTH_2, DEPTH_2), //blue #696969 105, 105, 105
		Scalar(DEPTH_3, DEPTH_3, DEPTH_3), //blue #696969 105, 105, 105
		Scalar(DEPTH_4, DEPTH_4, DEPTH_4), //blue #696969 105, 105, 105
	};

	int k, clusterCount = nclusters;
	int i, sampleCount = src.rows*src.cols;//样本数大小

	Mat points(sampleCount, 3, CV_32FC1), labels;   //产生的样本数，
	clusterCount = MIN(clusterCount, sampleCount);
	//centers = Mat(clusterCount, 1, points.type());    //用来存储聚类后的中心点

	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++){

			Vec3b ss = src.at<Vec3b>(i, j);
			points.at<float>(i*src.cols + j,0) = (float)ss[0];
			points.at<float>(i*src.cols + j,1) = (float)ss[1];
			points.at<float>(i*src.cols + j,2) = (float)ss[2];

		}

	//randShuffle(points, 1, &rng);   //因为要聚类，所以先随机打乱points里面的点，注意points和pointChunk是共用数据的。

	/*points: one rows per sample;type=float point matrix; for images rows=numPixels, col=channels
	  labels: size=[rows,1], record labels for each sample[row]
	  centers: size=[nclusters,cols], record ncluster centers
	  */
	cout << "k-means seg... " << endl;
	QtApi.sendMsg("\n k-means seg...     \n");
	clock_t skmeans = clock();
	kmeans(points, clusterCount, labels,
		TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0),

		3, KMEANS_PP_CENTERS, centers);  //聚类3次，取结果最好的那次，聚类的初始化采用PP特定的随机算法。
	clock_t ekmeans = clock();
	QtApi.sendMsg("[" + toString((double(ekmeans - skmeans) / CLOCKS_PER_SEC)) + " sec]\n");


	visual = src.clone();
	for (int i = 0; i < visual.rows; i++)
		for (int j = 0; j < visual.cols; j++){
			int clusterIdx = labels.at<int>(i*src.cols+j);
			int s0 = (int)centers.at<float>(clusterIdx, 0);
			int s1 = (int)centers.at<float>(clusterIdx, 1);
			int s2 = (int)centers.at<float>(clusterIdx, 2);

			Vec3b v = Vec3b(colorTab[clusterIdx][0], colorTab[clusterIdx][1], colorTab[clusterIdx][2]);
			//visual.at<Vec3b>(i, j) = Vec3b(s0, s1, s2); //center color
			visual.at<Vec3b>(i, j) = v; //my color
			result.at<uchar>(i, j) = clusterIdx;
		}

	_kmeans_res = result.clone();
	_kmeans_centers = centers.clone();
	_kmeans_ncluster = nclusters;
	_kmeans_nSamples = sampleCount;
	//namedWindow("cluster", 0);
	//imshow("cluster", visual); waitKey();
	return;
}

void Segmentation::save_kmeans(string dir){
	
	if (!boost::filesystem::exists(dir + _baseName))
		boost::filesystem::create_directories(dir + _baseName);

	if (boost::filesystem::exists(dir + _baseName + "\\labels.txt") &&
		boost::filesystem::exists(dir + _baseName + "\\centers.txt") &&
		boost::filesystem::exists(dir + _baseName + "\\scolor.txt") &&
	//	boost::filesystem::exists(dir + _baseName + "\\gPb.mat") &&
		boost::filesystem::exists(dir + _baseName + "\\label_weight.txt"))
		return;//already exist 

	ofstream ofs1(dir + _baseName + "\\labels.txt");
	ofstream ofs2(dir + _baseName + "\\centers.txt");
	ofstream ofs3(dir + _baseName + "\\scolor.txt");
	ofstream ofs4(dir + _baseName + "\\label_weight.txt");

	for (int i = 0; i < _kmeans_res.rows; i++){
		for (int j = 0; j < _kmeans_res.cols; j++)
			ofs1 << (int)_kmeans_res.at<uchar>(i, j) << " ";
		ofs1 << endl;
	}
		
		
	for (int i = 0; i < _kmeans_centers.rows; i++){
		for (int j = 0; j < _kmeans_centers.cols; j++)
			ofs2 << (int)_kmeans_centers.at<float>(i, j) << " ";
		ofs2 << endl;
	}

	for (int i = 0; i < _src.rows; i++){
		for (int j = 0; j < _src.cols; j++){
			ofs3 << (int)_src.at<Vec3b>(i, j)[0] << " "
				<< (int)_src.at<Vec3b>(i, j)[1] << " "
				<< (int)_src.at<Vec3b>(i, j)[2] << endl;
		}
		
	}

	for (int i = 0; i < _src.rows; i++){
		for (int j = 0; j < _src.cols; j++){
			Vec3b s = _src.at<Vec3b>(i, j);
			Point3i pix = Point3i((int)s[0], (int)s[1], (int)s[2]);
			for (int k = 0; k < _kmeans_centers.rows; k++){//calc distance between each pixel and each cluster center
				Point3i center = Point3i((int)_kmeans_centers.at<float>(k, 0), (int)_kmeans_centers.at<float>(k, 1), (int)_kmeans_centers.at<float>(k, 2));
				ofs4 << distanceP2P(pix,center) << " ";
			}
			ofs4 << endl;
		}
	
	}

	ofs1.close();
	ofs2.close();
	ofs3.close();
	ofs4.close();
	return;
}

void Segmentation::gco_seg(double smooth_sym, double smooth_grid) {
	double step_s = 0.05;
	double step_g = 2;
	/*for (double sym = 0; sym <= smooth_sym; sym += step_s)
		for (double grid = 1; grid <= smooth_grid; grid += step_g){*/
	for (double sym = 0; ; )
		for (double grid = 1; ; ){
			string label_file = "K-means\\" + _baseName + "\\labels.txt";
			string weight_file = "K-means\\" + _baseName + "\\label_weight.txt";
			//string gpb_file = "K-means\\" + _baseName + "\\gPb.txt";
			string gpb_file = "data\\training\\gPb-" + _baseName + ".txt";
			string color_file = "K-means\\" + _baseName + "\\scolor.txt";
			string center_file = "K-means\\" + _baseName + "\\centers.txt";
			
			//rsult
			string resultfile = "data\\result\\" + _baseName + "_result_label_s" + toString(sym) + "_g" + toString(grid) + ".txt";
			string simg = "data\\result\\" + _baseName + "_src_image.jpg";
			string slabel = "data\\result\\" + _baseName + "_src_label.jpg";
			string glabel = "data\\result\\" + _baseName + "_gco_label_s" + toString(sym) + "_g" + toString(grid) + ".jpg";

			//if (boost::filesystem::exists(resultfile) &&
			//	boost::filesystem::exists(simg) &&
			//	boost::filesystem::exists(slabel) &&
			//	//	boost::filesystem::exists(dir + _baseName + "\\gPb.mat") &&
			//	boost::filesystem::exists(glabel))
			//	continue;//already exist 
			
			cout << "\n============================GCO segmentation===========================\n" << endl <<
				"start gco segmentation for " << _imagePath << endl;
			QtApi.sendMsg("\n=============================GCO segmentation===========================\n" \
				"start gco segmentation for " + _imagePath + "\n");
			QtApi.sendMsg("image information: rows: " + toString(_src.rows) + " cols: " + toString(_src.cols) + "\n");
			QtApi.sendMsg("smooth_sym: " + toString(sym) + " smooth_grid: " + toString(grid) + "\n");
			//if (!init_gco){
			gcoSeg = new gcoUtil(_src.rows, _src.cols);

			gcoSeg->smooth_sym = sym;
			gcoSeg->smooth_grid = grid;

			cout << "initial data..." << endl;
			QtApi.sendMsg("initial data...    ");
			clock_t sinit = clock();
			gcoSeg->initial_data(label_file, weight_file, gpb_file, color_file, center_file, (_src.cols - 1) / 2);
			clock_t einit = clock();
			QtApi.sendMsg("[" + toString((double(einit - sinit) / CLOCKS_PER_SEC)) + " sec]\n");

			cout << "set neighbors..." << endl;
			QtApi.sendMsg("set neighbors...   ");
			clock_t snb = clock();
			gcoSeg->set_neighbors();
			clock_t enb = clock();
			QtApi.sendMsg("[" + toString((double(enb - snb) / CLOCKS_PER_SEC)) + " sec]\n");

			cout << "build graph... " << endl;
			QtApi.sendMsg("build graph...     ");
			clock_t sbuild = clock();
			gcoSeg->build_general_graph();
			clock_t ebuild = clock();
			QtApi.sendMsg("[" + toString((double(ebuild - sbuild) / CLOCKS_PER_SEC)) + " sec]\n");

			init_gco = true;
			//}
			/*else{
			gcoSeg->smooth_sym = v;
			gcoSeg->set_sym_neighbors();
			}*/
			cout << "gc solving..." << endl;
			QtApi.sendMsg("gc solving...      ");
			//QtApi.sendMsg("    symmetry smoothness = "+toString(smooth_sym));
			//QtApi.sendMsg("    grid smoothness = " + toString(smooth_grid));

			clock_t s1 = clock();
			gcoSeg->solve_general_graph();
			clock_t s2 = clock();
			QtApi.sendMsg("[" + toString((double(s2 - s1) / CLOCKS_PER_SEC)) + " sec]\n");
			//gco->read_labels("K-means\\" + _baseName + "\\labels.txt");
			//gco->read_label_center("K-means\\" + _baseName + "\\centers.txt");

			//visualization

			Scalar colorTab[] =     //最多只有10类，所以最多也就给10个颜色
			{
				Scalar(DEPTH_0, DEPTH_0, DEPTH_0), //blue #696969 105, 105, 105
				Scalar(DEPTH_1, DEPTH_1, DEPTH_1), //blue #696969 105, 105, 105
				Scalar(DEPTH_2, DEPTH_2, DEPTH_2), //blue #696969 105, 105, 105
				Scalar(DEPTH_3, DEPTH_3, DEPTH_3), //blue #696969 105, 105, 105
				Scalar(DEPTH_4, DEPTH_4, DEPTH_4), //blue #696969 105, 105, 105
			};

			//namedWindow("before optimization", 0);
			//namedWindow("after optimization", 0);
			
			ofstream ofs(resultfile);

			Mat be = _src.clone();
			Mat af = _src.clone();

			for (int i = 0; i < _src.rows; i++){
				for (int j = 0; j < _src.cols; j++){
					int label_b = (int)gcoSeg->init_labels(i, j);
					int label_a = (int)gcoSeg->result_labels(i, j);

					ofs << label_a << " ";

					int s0 = (int)colorTab[label_b][0];
					int s1 = (int)colorTab[label_b][1];
					int s2 = (int)colorTab[label_b][2];

					int s00 = (int)colorTab[label_a][0];
					int s11 = (int)colorTab[label_a][1];
					int s22 = (int)colorTab[label_a][2];

					Vec3b v1 = Vec3b(s0, s1, s2);
					Vec3b v2 = Vec3b(s00, s11, s22);

					be.at<Vec3b>(i, j) = v1;
					af.at<Vec3b>(i, j) = v2;

				}
				ofs << endl;
			}

			_dst = af.clone();

			cout << "writing to" << simg << endl
				<< "writing to" << slabel << endl
				<< "writing to" << glabel << endl;
			QtApi.sendMsg("writing to " + simg + "\n" \
				"writing to " + slabel + "\n" \
				"writing to " + glabel + "\n");

			imwrite(simg, _src);
			imwrite(slabel, be);
			imwrite(glabel, af);
			//imshow("before optimization", be);
			//imshow("after optimization", af);
			//waitKey();
			delete gcoSeg;

			cout << "gco segmentation for " << _imagePath << " done" << endl <<
				"=======================END GCO segmentation============================\n" << endl << endl << endl;
			QtApi.sendMsg("gco segmentation for " + _imagePath + " done\n" \
				"=======================END GCO segmentation==============================\n");
		}
}

void Segmentation::gco_segAll(string inputDir, string outDir, double smooth_sym, double smooth_grid){
	if (!boost::filesystem::exists(inputDir)){
		//emit message("src directory not exists\n");
		cout << "src directory not exists"<<endl;
		return;
	}

	fs::path fullpath(inputDir, fs::native);
	if (!fs::exists(fullpath)) return;

	fs::recursive_directory_iterator end_iter;
	for (fs::recursive_directory_iterator iter(fullpath); iter != end_iter; iter++)
	{
		if (!fs::is_directory(*iter)){
			//cout << "current image" << iter->path().string() << endl;
			_imagePath = iter->path().string();
			string currentImageS = iter->path().filename().string();
			_baseName = currentImageS.substr(0, currentImageS.find_first_of('.'));
			string ext = currentImageS.substr(currentImageS.find_first_of('.') + 1, currentImageS.length() - 1);
			if (ext == "jpg"){
				Mat img = imread(_imagePath, 1);
				_src = img;
				gco_seg(smooth_sym, smooth_grid);
			}
			else continue;

		}
	}
}

void Segmentation::loadImages(string inputDir){
	if (!boost::filesystem::exists(inputDir)){
		//emit message("src directory not exists\n");
		cout << inputDir + "not exists" << endl;
		return;
	}
	QtApi.sendMsg("start labeling files for " + inputDir + "*.jpg\n");

	fs::path fullpath(inputDir, fs::native);
	fs::recursive_directory_iterator end_iter;
	for (fs::recursive_directory_iterator iter(fullpath); iter != end_iter; iter++)
	{
		if (!fs::is_directory(*iter)){
			//cout << "current image" << iter->path().string() << endl;
			_imagePath = iter->path().string();
			string currentImageS = iter->path().filename().string();
			_baseName = currentImageS.substr(0, currentImageS.find_first_of('.'));
			string ext = currentImageS.substr(currentImageS.find_first_of('.') + 1, currentImageS.length() - 1);
			if (ext == "jpg"){
				imagePath.push_back(_imagePath);
				baseNames.push_back(_baseName);
			}
			else continue;

		}
	}

	QtApi.sendMsg("load " + toString((int)_imagePath.size()) + " jpg images\n");
	return;
}

bool Segmentation::labelNext(){
	cursor++;
	if (cursor < imagePath.size()){
		_src = imread(imagePath[cursor]);
		_baseName = baseNames[cursor];
		return true;
	}
		
	QtApi.sendMsg("No more images\n");
	return false;
}

bool Segmentation::labelPrevious(){
	if (imagePath.size() == 0){
		QtApi.sendMsg("No previous images\n");
		return false;
	}

	cursor--;
	if (cursor < imagePath.size()){
		_src = imread(imagePath[cursor]);
		_baseName = baseNames[cursor];
		return true;
	}

}

void Segmentation::userLabel(QPoint tl, QPoint br){
	for (int i = tl.y(); i < br.y(); i++)
		for (int j = tl.x(); j < br.x(); j++){
		if (_src.at<Vec3b>(i, j) != Vec3b(DEPTH_0, DEPTH_0, DEPTH_0) && _src.at<Vec3b>(i, j) != Vec3b(DEPTH_1, DEPTH_1, DEPTH_1) &&
			_src.at<Vec3b>(i, j) != Vec3b(DEPTH_2, DEPTH_2, DEPTH_2) && _src.at<Vec3b>(i, j) != Vec3b(DEPTH_3, DEPTH_3, DEPTH_3) &&
			_src.at<Vec3b>(i, j) != Vec3b(DEPTH_4, DEPTH_4, DEPTH_4))
			_src.at<Vec3b>(i, j) = Vec3b(depth, depth, depth);
		}
	//imshow("user", _src); waitKey(0);
}

void Segmentation::saveLabels(string labelDir){
	ofstream ofs(labelDir + _baseName + "_label.txt");
	for (int i = 0; i < _src.rows; i++)
		for (int j = 0; j < _src.cols; j++){
		if (_src.at<Vec3b>(i, j) != Vec3b(DEPTH_0, DEPTH_0, DEPTH_0) && _src.at<Vec3b>(i, j) != Vec3b(DEPTH_1, DEPTH_1, DEPTH_1) &&
			_src.at<Vec3b>(i, j) != Vec3b(DEPTH_2, DEPTH_2, DEPTH_2) && _src.at<Vec3b>(i, j) != Vec3b(DEPTH_3, DEPTH_3, DEPTH_3) &&
			_src.at<Vec3b>(i, j) != Vec3b(DEPTH_4, DEPTH_4, DEPTH_4))
			_src.at<Vec3b>(i, j) = Vec3b(DEPTH_4, DEPTH_4, DEPTH_4);
		}
	if (!boost::filesystem::exists(labelDir + _baseName + "_label.jpg") && !boost::filesystem::exists(labelDir + _baseName + "_label.txt")){
		for (int i = 0; i < _src.rows; i++){
			for (int j = 0; j < _src.cols; j++){
				if (_src.at<Vec3b>(i, j) == Vec3b(DEPTH_0, DEPTH_0, DEPTH_0))
					ofs << "0 ";
				else if (_src.at<Vec3b>(i, j) == Vec3b(DEPTH_1, DEPTH_1, DEPTH_1))
					ofs << "1 ";
				else if (_src.at<Vec3b>(i, j) == Vec3b(DEPTH_2, DEPTH_2, DEPTH_2))
					ofs << "2 ";
				else if (_src.at<Vec3b>(i, j) == Vec3b(DEPTH_3, DEPTH_3, DEPTH_3))
					ofs << "3 ";
				else
					ofs << "4 ";
			}
			ofs << endl;
		}
			
		imwrite(labelDir + _baseName + "_label.jpg", _src);
	}
		
	
	else{
		QtApi.sendMsg(labelDir + _baseName + "_label.jpg already exist...");
	}
}