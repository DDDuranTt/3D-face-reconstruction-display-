#include <Eigen/Dense>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "Model.h"
#include "PlyReader.h"
#include "PlyWriter.h"
#include "myMath.h"

struct dotAndFaceIndex {
	float dot = 0.0f;
	int faceIndex;
};

int SeleteFaceWithFunction(float k1, float b1, float k2, float b2, Model& model, std::unordered_set<int>* setLst) {//根据函数分区
	dotAndFaceIndex temp;
	float pointX, pointZ;
	int flag[3];
	int finalFlag;
	for (int i = 0; i < model.faceNum; ++i) {
		temp.faceIndex = i;

		for (int j = 0; j < 3; ++j)//对构成面的三个点进行分区
		{
			pointX = model.vertices[model.faces[i].index(j)].position(0);
			pointZ = model.vertices[model.faces[i].index(j)].position(2);

			if (pointX * k1 + b1 < pointZ && pointX * k2 + b2 < pointZ) {//正脸
				flag[j] = 1;
			}
			else if (pointX * k1 + b1 > pointZ && pointX * k2 + b2 < pointZ) {//左脸
				flag[j] = 2;
			}
			else if (pointX * k1 + b1 < pointZ && pointX * k2 + b2 > pointZ) {//右脸
				flag[j] = 3;
			}
			else {
				flag[j] = 0;
				std::cout << "[Warning] 点[Index: " << i << "]未落在有效区域，请继续调整函数" << std::endl;
				return 0;
			}
		}
		//根据三个顶点的落点最终确定其所构成的面属于哪个分区
		if (flag[0] == flag[1])finalFlag = flag[0];
		else {
			if (flag[1] == flag[2])finalFlag = flag[1];
			else finalFlag = flag[0];
		}

		setLst[finalFlag - 1].insert(i);
	}
	return 1;
}

void SelectFacePostProcess(std::unordered_set<int>* setLst, Eigen::Vector3f& centerView, Eigen::Vector3f& leftView, Eigen::Vector3f& rightView, Model& model, float
	threshold) {
	Eigen::Vector3f faceNormal;
	std::vector<int> needToDelete;
	float dot;
	needToDelete.reserve(setLst[0].size());
	for (auto it = setLst[0].begin(); it != setLst[0].end(); ++it) //处理中间分区
	{
		faceNormal = Eigen::Vector3f::Zero();
		faceNormal = model.vertices[model.faces[*it].index(0)].normal //第i个面的第一个顶点
			+ model.vertices[model.faces[*it].index(1)].normal //第i个面的第二个顶点
			+ model.vertices[model.faces[*it].index(2)].normal;//第i个面的第三个顶点
		faceNormal.normalize();
		dot = faceNormal.dot(centerView);

		if (dot < threshold) {
			if (model.vertices[model.faces[*it].index(0)].position(0) > 0) { // X大于零在左脸
				setLst[1].insert(*it);
				needToDelete.push_back(*it);
			}
			else {
				setLst[2].insert(*it);
				needToDelete.push_back(*it);
			}
		}
	}

	for (int i = 0; i<needToDelete.size();++i)
	{
		setLst[0].erase(needToDelete[i]);
	}
	needToDelete.clear();
	//=====================================================
	needToDelete.reserve(setLst[1].size());
	for (auto it = setLst[1].begin(); it != setLst[1].end(); ++it) //处理左间分区
	{
		faceNormal = Eigen::Vector3f::Zero();
		faceNormal = model.vertices[model.faces[*it].index(0)].normal //第i个面的第一个顶点
			+ model.vertices[model.faces[*it].index(1)].normal //第i个面的第二个顶点
			+ model.vertices[model.faces[*it].index(2)].normal;//第i个面的第三个顶点
		faceNormal.normalize();
		dot = faceNormal.dot(leftView);

		if (dot < threshold) {
				setLst[0].insert(*it);
				needToDelete.push_back(*it);
		}
	}

	for (int i = 0; i < needToDelete.size(); ++i)
	{
		setLst[1].erase(needToDelete[i]);
	}
	needToDelete.clear();
	//============================================================
	needToDelete.reserve(setLst[0].size());
	for (auto it = setLst[2].begin(); it != setLst[2].end(); ++it) //处理右分区
	{
		faceNormal = Eigen::Vector3f::Zero();
		faceNormal = model.vertices[model.faces[*it].index(0)].normal //第i个面的第一个顶点
			+ model.vertices[model.faces[*it].index(1)].normal //第i个面的第二个顶点
			+ model.vertices[model.faces[*it].index(2)].normal;//第i个面的第三个顶点
		faceNormal.normalize();
		dot = faceNormal.dot(rightView);

		if (dot < threshold) {
				setLst[0].insert(*it);
				needToDelete.push_back(*it);
		}
	}

	for (int i = 0; i < needToDelete.size(); ++i)
	{
		setLst[2].erase(needToDelete[i]);
	}
	needToDelete.clear();
}
//void SelectFace(Model& model, Eigen::Vector3f& viewDir, int belongID, std::unordered_map<int, dotAndBelongID>& dotMap) {
//	dotAndBelongID temp;
//	Eigen::Vector3f faceNormal;
//	std::unordered_map<int, dotAndBelongID>::iterator it;
//	float dot;
//	for (size_t i = 0; i < model.faceNum; ++i)
//	{
//
//		faceNormal = Eigen::Vector3f::Zero();
//		faceNormal = model.vertices[model.faces[i].index(0)].normal //第i个面的第一个顶点
//			+ model.vertices[model.faces[i].index(1)].normal //第i个面的第二个顶点
//			+ model.vertices[model.faces[i].index(2)].normal;//第i个面的第三个顶点
//
//		faceNormal.normalize();
//		dot = faceNormal.dot(viewDir);
//		it = dotMap.find(i);
//
//		if (it == dotMap.end()) {
//			temp.dot = dot;
//			temp.belongId = belongID;
//			dotMap.insert(std::make_pair(i, temp));
//		}
//		else if (it != dotMap.end() && it->second.dot < dot) {
//			it->second.belongId = belongID;
//		}
//	}
//}
//
//void SelectFace(Model* mod, Eigen::Vector3f& centerView, Eigen::Vector3f& leftView, Eigen::Vector3f& rightView, std::unordered_set<dotAndFaceIndex>* setLst) {
//	SelectFace(*mod, centerView, 1, dotMap);
//	SelectFace(*mod, leftView, 2, dotMap);
//	SelectFace(*mod, rightView, 3, dotMap);
//}

void UVPostProcess(float** pictureSize, int belongID, vertex& ver) {
	float horizontal_Maxp_Pixel = pictureSize[0][0] + pictureSize[1][0] + pictureSize[2][0];
	float vertical_Maxp_Pixel = pictureSize[0][1];
	if (belongID == 1) {
		ver.uv(0) = ver.uv(0) * pictureSize[0][0] / horizontal_Maxp_Pixel;
	}
	else if (belongID == 2) {
		ver.uv(0) = (ver.uv(0) * pictureSize[1][0] + pictureSize[0][0]) / horizontal_Maxp_Pixel;
		ver.uv(1) = ver.uv(1) * pictureSize[1][1] / vertical_Maxp_Pixel;
	}
	else if (belongID == 3) {
		ver.uv(0) = (ver.uv(0) * pictureSize[2][0] + pictureSize[0][0] + pictureSize[1][0]) / horizontal_Maxp_Pixel;
		ver.uv(1) = ver.uv(1) * pictureSize[2][1] / vertical_Maxp_Pixel;
	}
}

void ComputeUV(std::unordered_set<int>* setLst, mat4* projMatLst, Model& myModel, float** pictureSize) {
	std::unordered_map<int, int> visitedVertex;/*存储被访问过的点的index*/
	std::unordered_map<int, int>::iterator it_visitedVertex;
	vertex temp;
	for (int i = 0; i < 3; ++i)//对三个面的分区进行操作
	{
		for (auto it = setLst[i].begin(); it != setLst[i].end(); ++it) //对每个三角面进行操作
		{
			for (int j = 0; j < 3; ++j) //对每个顶点进行操作
			{
				//判断该顶点是否访问过，且两次访问是否属于同一角度（已访问过表示已经存在纹理坐标）。若不属于同一角度，则在与原点相同的位置新生成一个点，保存另外一个角度下的纹理坐标
				it_visitedVertex = visitedVertex.find(myModel.faces[*it].index(j));
				if (it_visitedVertex != visitedVertex.end() && it_visitedVertex->second != i) {
					temp = myModel.vertices[myModel.faces[*it].index(j)];
					myModel.vertices.push_back(temp);//新点压倒队尾
					++myModel.vertexNum;
					myModel.faces[*it].index(j) = myModel.vertices.size() - 1;//修改该面的顶点索引
				}
				else {
					visitedVertex.insert(std::make_pair(myModel.faces[*it].index(j), i));
				}

				ComputeUV(myModel.vertices[myModel.faces[*it].index(j)], projMatLst[i]);

				UVPostProcess(pictureSize, i + 1, myModel.vertices[myModel.faces[*it].index(j)]);
			}
		}
	}
}

int main(int argc, char** argv) {
	Model* myModel = ReadFile("D:/pointCloud/mine/concat_recon_7_left_post.ply");;

	myModel->Normalized();

	//建立三个相机的视角向量
	Eigen::Vector3f centerCameraView = Eigen::Vector3f::Identity();
	Eigen::Vector3f leftCameraView = Eigen::Vector3f::Identity();
	Eigen::Vector3f rightCameraView = Eigen::Vector3f::Identity();

	centerCameraView(0) = 0;
	centerCameraView(1) = 0;
	centerCameraView(2) = -1;

	Eigen::Matrix3f	transformation_matrix = Eigen::Matrix3f::Identity();

	transformation_matrix(0, 0) = 0.776184;
	transformation_matrix(1, 0) = 0.004099;
	transformation_matrix(2, 0) = 0.630493;


	transformation_matrix(0, 1) = -0.013670;
	transformation_matrix(1, 1) = 0.999853;
	transformation_matrix(2, 1) = 0.010329;


	transformation_matrix(0, 2) = -0.630358;
	transformation_matrix(1, 2) = -0.016636;
	transformation_matrix(2, 2) = 0.776126;

	transformation_matrix = transformation_matrix.transpose().eval();
	transformation_matrix = transformation_matrix.inverse().eval();
	rightCameraView = transformation_matrix * centerCameraView;

	transformation_matrix = Eigen::Matrix3f::Identity();

	transformation_matrix(0, 0) = 0.787938;
	transformation_matrix(1, 0) = 0.000293;
	transformation_matrix(2, 0) = -0.615752;

	transformation_matrix(0, 1) = -0.005540;
	transformation_matrix(1, 1) = 0.999962;
	transformation_matrix(2, 1) = -0.006616;

	transformation_matrix(0, 2) = 0.615727;
	transformation_matrix(1, 2) = 0.008625;
	transformation_matrix(2, 2) = 0.787910;
	transformation_matrix = transformation_matrix.transpose().eval();
	transformation_matrix = transformation_matrix.inverse().eval();

	leftCameraView = transformation_matrix * centerCameraView;

	centerCameraView *= -1;
	leftCameraView *= -1;
	rightCameraView *= -1;

	//选出顶点
	std::unordered_set<int>* setLst = new std::unordered_set<int>[3];//三个集合分别是对应的中间相机选中的面、左相机选中的面和右相机选中的面
	if (!SeleteFaceWithFunction(2.0f, -400.0f, -1.8f, -400.0f, *myModel, setLst)) {
		return 0;
	}
	//SelectFacePostProcess(setLst, centerCameraView, leftCameraView, rightCameraView, *myModel, 0.5);
	//SelectFace(myModel, centerCameraView, leftCameraView, rightCameraView, setLst);

	std::cout << "正脸选中：" << setLst[0].size() << '\n' << "左脸选中：" << setLst[1].size() << '\n' << "右脸选中：" << setLst[2].size() << std::endl;

	//std::cout << " |已选中: " << dotMap.size() << " |总数: " << myModel->faceNum
	//	<< " |选中占比: " << (double)dotMap.size() / (double)myModel->faceNum * 100 << "%" << std::endl;


	//三个方向的投影矩阵
	mat4 ProjLst[3];//分别时中间、左边、右边的投影矩阵;

	if (!ReadMat("D:/pointCloud/mine/ProjectionMatrix/centerMat.txt", ProjLst[0])) {
		std::cout << "[Error] can't read mat" << std::endl;
		return 0;
	}


	if (!ReadMat("D:/pointCloud/mine/ProjectionMatrix/leftMat.txt", ProjLst[1])) {
		std::cout << "[Error] can't read mat" << std::endl;
		return 0;
	}

	if (!ReadMat("D:/pointCloud/mine/ProjectionMatrix/rightMat.txt", ProjLst[2])) {
		std::cout << "[Error] can't read mat" << std::endl;
		return 0;
	}

	//计算三个相机视角下，该视角所选顶点的二维坐标
	float** pictureSize = new float* [3];
	pictureSize[0] = new float[2];
	pictureSize[1] = new float[2];
	pictureSize[2] = new float[2];

	pictureSize[0][0] = 2160.0f; pictureSize[0][1] = 3840.0f;
	pictureSize[1][0] = pictureSize[2][0] = 2448.0f;
	pictureSize[1][1] = pictureSize[2][1] = 3264.0f;

	ComputeUV(setLst, ProjLst, *myModel, pictureSize);

	delete pictureSize[0];
	delete pictureSize[1];
	delete pictureSize[2];
	delete pictureSize;

	WriteFile("D:/pointCloud/image/dotSelect.ply", *myModel);

	delete myModel;
	return 0;
}