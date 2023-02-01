#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


template<typename T>
T product(vector<T> & arr) {
    T cumprod = 1;
    for(int i = 0; i < arr.size(); ++i)
        cumprod *= arr[i];
    return cumprod;
}


vector<int> calc_stride(vector<int> & shape) {
    int p = 1;
    int n = shape.size();
    vector<int> stride(n);

    for(int i = n - 1; i >= 0; --i) {
        stride[i] = p;
        p *= shape[i];
    }

    return stride;
}


template<typename T>
class Matrix {
private:
    vector<T> inner;
    vector<int> shape, stride;
    int dim, size;

    int get_index(vector<int> & path) {
        if(path.size() != dim) return -1;
        int idx = 0;
        for(int i = 0; i < path.size(); ++i) {
            if (path[i] < 0 || path[i] >= shape[i])
                return -1;
            idx += path[i] * stride[i];
        }
        return idx;
    }

public:
    Matrix(vector<int> shape) {
        this->dim = shape.size();
        this->shape = shape;
        this->size = product(shape);
        this->inner = vector<T>(size);
        this->stride = calc_stride(shape);
    }

    void set_inner(vector<T> & new_inner) {
        inner = new_inner;
    }

    void set(vector<int> path, T value) {
        int idx = get_index(path);
        inner[idx] = value;
        cout << "SET " << idx << endl;
    }

    T item() {
        return inner[0];
    }

    Matrix select(vector<vector<int>> indices) {
        vector<int> new_shape(dim);
        vector<int> new_inner;

        for(int i = 0; i < indices.size(); ++i)
            new_shape[i] = indices[i].size();

        int i, b, br;
        i = 0;

        while (i < inner.size()) {
            br = 0;
            cout << "INNER " << i << endl;
            for (int j = 0; j < indices.size(); ++j) {
                b = i / stride[j];
                b = j == 0? b : b % shape[j];

                cout << "Checking " << j << " dim" << endl;

                if (find(indices[j].begin(),
                         indices[j].end(),
                         b) != indices[j].end()) continue;

                i += stride[j];
                cout << "Skipping to " << i << endl;
                br = 1;
                break;
            }
            if (br) continue;
            new_inner.push_back(inner[i]);
            ++i;
        }
        cout << new_inner.size() << endl;
        Matrix new_mat(new_shape);
        new_mat.set_inner(new_inner);
        return new_mat;
    }

    T get(vector<int> path) {
        int idx = get_index(path);
        cout << "GET " << idx << endl;
        return inner[idx];
    }
};

int main()
{
    Matrix<int> b({50, 300, 400});
    b.select({{0}, {0, 1}, {0, 1}});
    return 0;
}






