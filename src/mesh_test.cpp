#include <iostream>
#include "cyTriMesh.h"

using namespace std;

/*int main(int argc, char const *argv[]){
	
	cy::TriMesh mesh;
	if(mesh.LoadFromFileObj(argv[1], false)){

		cout << "[INFO ==> Nº TRIANGLES] " << mesh.NF() << std::endl;
		cout << "[INFO ==> Nº VERTICES] " << mesh.NV() << std::endl;
		cout << "[INFO ==> INDICES]";
		for(int i = 0; i < mesh.NF(); ++i){
			cy::TriMesh::TriFace f = mesh.F(i);
			for(int j = 0; j < 3; ++j)
				cout << " " << f.v[j];
		}
		cout << endl;

		cout << "[INFO ==> VERTICES]";
		for(int i = 0; i < mesh.NV(); ++i){
			cy::Point3f p = mesh.V(i);
			for(int j = 0; j < 3; ++j)
				cout << " " << p[j];
		}
		cout << endl;


		if(mesh.NVN() != mesh.NV()){
			mesh.ComputeNormals(true);	
		}

		cout << "[INFO ==> Nº NORMALS] " << mesh.NVN() << std::endl;
		cout << "[INFO ==> NORMALS]";
		for(int i = 0; i < mesh.NVN(); ++i){
			cy::Point3f p = mesh.VN(i);
			for(int j = 0; j < 3; ++j)
				cout << " " << p[j];
		}
		cout << endl;

	}else{
		cout << "Fail in loading\n";
	}

	return 0;
}*/