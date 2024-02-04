#include <iostream>
#include <cstring>
#include <dirent.h>
#include <vector>
#include "Exp.h"
using namespace std;

int main() {
    Exp* exp;
	short datanum, len, i, j, k, num, kind;
	float tacc, tfit;
    DIR *dirp;
    struct dirent *dp;

    // Dataディレクトリを開く
    dirp = opendir("Data");
    if (dirp == nullptr) {
        cerr << "Error: Could not open directory 'Data'" << endl;
        return 1;
    }

    // ファイル名を保存するための動的なvectorを宣言
    vector<string> filenames;

    while ((dp = readdir(dirp)) != nullptr) {
        // ファイル名が.txtで終わるかどうかを確認
        std::string fname(dp->d_name);
        if (fname.size() > 4 && fname.substr(fname.size() - 4) == ".txt") {
            // ".txt" を削除してファイル名だけを保存
            filenames.push_back(fname.substr(0, fname.size() - 4));
        }
    }

    // ディレクトリストリームを閉じる
    closedir(dirp);

    // char **dataFnameにファイル名を格納
    char **dataFname = new char*[filenames.size()];
    for (size_t i = 0; i < filenames.size(); ++i) {
        dataFname[i] = new char[filenames[i].size() + 1];
        strcpy(dataFname[i], filenames[i].c_str());
    }

    /*
    // 結果を出力（デバッグ用）
    for (size_t i = 0; i < filenames.size(); ++i) {
        cout << dataFname[i] << endl;
    }
    */
    datanum = filenames.size();

    for(i = 0; i < datanum; i++) {
        cout << "NO\tCROSS\t世代\t正解率\t木の深さ\tノード数\t適応度\t訓練時間\t訓練正解率\t訓練適応度\n";
        k = 0;
        exp = new Exp(dataFname[i], "parameters.txt");
        for(j = 0; j < 1/*exp->para->CrossValNum*/; j++) {
            exp->initialize(j);
            exp->training();
            cout << "test--------------------" << endl;
            exp->test();
        }
        delete exp;
    }

    // メモリを解放
    for (size_t i = 0; i < filenames.size(); ++i) {
        delete[] dataFname[i];
    }
    delete[] dataFname;

    return 0;
}
