#include "BrowDir.h"
#include <opencv2/opencv.hpp>
using namespace cv;

Mat norm0_250(const Mat& src)
{
	Mat dst;
	switch(src.channels())
	{
	case 1:
		normalize(src, dst, NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		normalize(src, dst, NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
	}
	return dst;
}
Mat makerowdata(const vector<string>& filenames)
{
	CV_Assert(filenames.size() != 0);
	size_t n = filenames.size();
	vector<Mat> matvec;
	for (int i = 0; i != n; ++i)
	{
		Mat temp = imread(filenames[i], CV_LOAD_IMAGE_ANYDEPTH);
		if (!temp.data)
		{
			continue;
		}
		matvec.push_back(temp);
	}
	size_t d = matvec[0].total();
	Mat data(n, d, matvec[0].type());
	for (int i = 0; i != n; ++i)
	{
		if (matvec[i].total() != d)
		{
			cerr<<"Error: The total() of img is not the same size!"<<endl;
			return Mat();
		}
		if (matvec[i].isContinuous())
			matvec[i].reshape(1, 1).copyTo(data.row(i));
		else
			matvec[i].clone().reshape(1, 1).copySize(data.row(i));
	}
	return data;
}
int main(int argc, char** argv)
{
	char trainpath[_MAX_PATH] = "D:\\Documents\\Visual Studio 2012\\Projects\\����ʶ��\\eigenfacesdetect\\eigenfacesdetect\\trainning data";
	char testpath[_MAX_PATH] = "D:\\Documents\\Visual Studio 2012\\Projects\\����ʶ��\\eigenfacesdetect\\eigenfacesdetect\\testing data";
	if (_chdir(trainpath) == -1)
	{
		cerr<<"This is not a filepath available!"<<endl;
		return -1;
	}
	CBrowseDir browsedir;
	if (!browsedir.SetInitDir(trainpath))
	{
		cerr<<"Set filepath failed!"<<endl;
		return -1;
	}
	//����Ŀ¼�ҵ�ָ����׺������ͼ��
	vector<string> filenames = browsedir.BeginBrowseFilenames("*.bmp*");
	Mat sample = imread(filenames[0], CV_LOAD_IMAGE_ANYDEPTH);
	//��ȡ������Ч��ѵ������
	Mat data = makerowdata(filenames);
	//PCA
	int num_compents = 10;
	PCA pca(data, Mat(), CV_PCA_DATA_AS_ROW, num_compents);
#if 0
	Mat mean = pca.mean.clone();
	imshow("mean", norm0_250(mean.reshape(1, sample.rows)));
	for (int i = 0; i != data.rows; ++i)
	{
		string windowname = format("pca_sample%d", i);
		imshow(windowname, norm0_250(pca.eigenvectors.row(i).reshape(1, sample.rows)));
		waitKey(0);
	}
#endif
	//�ռ����в�����������Ϣ
	filenames.clear();
	if (_chdir(testpath) == -1)
	{
		cerr<<"Error:This is not a filepath available!"<<endl;
		return -1;
	}
	if (!browsedir.SetInitDir(testpath))
	{
		cerr<<"Set filepath failed!"<<endl;
		return -1;
	}
	filenames = browsedir.BeginBrowseFilenames("*.bmp*");
	data.release();
	data = makerowdata(filenames);
	//compressed����ÿ������������ͶӰ�������ӿռ��Ȩ��
	Mat compressed(data.rows, num_compents, data.type());
	//recontructed����ÿ�����������������ӿռ��ؽ��õ���ͼ��
	Mat recontructed;
	for (int i = 0; i != data.rows; ++i)
	{
		string windowname = format("test_sample_recontruct%d", i);
		Mat vec = data.row(i), coeffs = compressed.row(i);
		pca.project(vec, coeffs);
		pca.backProject(coeffs, recontructed);
		vec.convertTo(vec, CV_32FC1);
		cout<<"testdata("<<i<<")"<<"diff = "<<norm(vec, recontructed, NORM_L2)<<endl;
		//imshow("test_sample", norm0_250(vec).reshape(1, sample.rows));
		//imshow(windowname, norm0_250(recontructed).reshape(1, sample.rows));
		//waitKey(0);
	}
	return 0;
}