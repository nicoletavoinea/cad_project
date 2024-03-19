#ifndef FLC_H
#define FLC_H

#include <omnetpp.h>
#include <string>

#define MAX_NR_TERMS 16
#define MAX_NR_INP 20
using namespace omnetpp;

class Vector {
public:
    Vector(int m);
    ~Vector();
    int get_m() const;
    int get(int j) const;
    void set(int pos, int val);
    void print();
    void min_vect(Vector* v2, Vector* result);

private:
    int _m;
    int* vect;
};

class Matrix {
public:
    Matrix(int m, int n);
    ~Matrix();
    int get_m() const;
    int get_n() const;
    void set(int i, int j, int val);
    int get(int i, int j) const;
    void print();
    void max_column(Vector* v);
    void min_matrix(Matrix* m2, Matrix* result);
    void max_matrix(Matrix* m2, Matrix* result);
    void min_vector(Vector* v, Matrix* result);

private:
    int _m;
    int _n;
    int** mat;
};

class Term {
public:
    Term(int a, int b, int c, int d, std::string name);
    ~Term();
    std::string getName() const;
    int getTermRep(int m, int n, Matrix* mat);

private:
    std::string _name;
    int _a, _b, _c, _d;
};

class FLC : public cSimpleModule {
public:
    //FLC();
    virtual ~FLC();
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    int fuzzy_inference(int* inp, int nb_inp, int delta);
    int W_HP;

private:
    int bits_n; // number of binary vectors representing a membership function
    int bits_m; // number of bits in each vector /
    int n_inp;  // number of inputs /
    int n_rules;  // number of rules /
    int nb_terms[MAX_NR_INP+1];
    Term* m_functions[MAX_NR_INP+1][MAX_NR_TERMS]; /* membership functions for input and output */
    int** rules;
    cOutVector qtime,qtimew;
    cOutVector res_dep, result_dep;
    Matrix* fuzzify(int crisp_in, int delta);
    int defuzify(Matrix* mat);
    double scale(double n, double m, double a, double b, double x);
};

#endif // FLC_H
