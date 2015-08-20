#include "jit.common.h"

struct HullPt {
    double x, y, z;
    HullPt *prev, *next;
    void act() {
        if (prev->next != this) prev->next = next->prev = this;  // insert
        else { prev->next = next; next->prev = prev; }  // delete
    }
};


const double INF = 1e99;
static HullPt hNIL = {INF, INF, INF, 0, 0};
HullPt *hhNIL = &hNIL;

inline double turn(HullPt *p, HullPt *q, HullPt *r) {  // <0 iff cw
    if (p == hNIL || q == hNIL || r == hNIL) return 1.0;
    return (q->x-p->x)*(r->y-p->y) - (r->x-p->x)*(q->y-p->y);
}

inline double time(HullPt *p, HullPt *q, HullPt *r) {  // when turn changes
    if (p == hNIL || q == hNIL || r == hNIL) return INF;
    return ((q->x-p->x)*(r->z-p->z) - (r->x-p->x)*(q->z-p->z)) / turn(p,q,r);
}

HullPt *sort(HullPt P[], int n) {  // mergesort
    HullPt *a, *b, *c, head;
    
    if (n == 1) { P[0].next = hNIL; return P; }
    a = sort(P, n/2);
    b = sort(P+n/2, n-n/2);
    c = &head;
    do
        if (a->x < b->x) { c = c->next = a; a = a->next; }
        else { c = c->next = b; b = b->next; }
    while (c != hNIL);
    return head.next;
}

void hull(HullPt *list, int n, HullPt **A, HullPt **B) {  // the algorithm
    
    HullPt *u, *v, *mid;
    double t[6], oldt, newt;
    int i, j, k, l, minl;
    
    if(n==1){
        A[0] = list->prev = list->next = hNIL;
        return;
    }
    
    for(u=list,i= 0; i < n/2-1; u = u->next, i++) ;
    mid = v = u->next;
    hull(list, n/2, B, A);  // recurse on left and right sides
    hull(mid, n-n/2, B+n/2*2, A+n/2*2);
    
    for ( ; ; )  // find initial bridge
        if (turn(u, v, v->next) < 0) v = v->next;
        else if (turn(u->prev, u, v) < 0) u = u->prev;
        else break;
    
    // merge by tracking bridge uv over time
    for(i=k=0,j=n/2*2,oldt=-INF;;oldt=newt)
    {
        t[0] = time(B[i]->prev, B[i], B[i]->next);
        t[1] = time(B[j]->prev, B[j], B[j]->next);
        t[2] = time(u, u->next, v);
        t[3] = time(u->prev, u, v);
        t[4] = time(u, v->prev, v);
        t[5] = time(u, v, v->next);
        for(newt=INF,l=0;l < 6; l++)
            if (t[l] > oldt && t[l] < newt) { minl=l;newt=t[l]; }
        if (newt == INF) break;
        switch (minl) {
            case 0:  if (B[i]->x < u->x) A[k++] = B[i];  B[i++]->act();  break;
            case 1:  if (B[j]->x > v->x) A[k++] = B[j];  B[j++]->act();  break;
            case 2:  A[k++] = u = u->next;  break;
            case 3:  A[k++] = u;  u = u->prev;  break;
            case 4:  A[k++] = v = v->prev;  break;
            case 5:  A[k++] = v;  v = v->next;  break;
        }
    }
    A[k] = hNIL;

    u->next = v;  v->prev = u;  // now go back in time to update HullPters
    for (k--; k >= 0; k--)
    if (A[k]->x <= u->x || A[k]->x >= v->x) {
        A[k]->act();
        if (A[k] == u) u = u->prev; else if (A[k] == v) v = v->next;
    }
    else {
        u->next = A[k]; A[k]->prev = u; v->prev = A[k]; A[k]->next = v;
        if (A[k]->x < mid->x) u = A[k]; else v = A[k];
    }
        
}

int main()
{
    int n, i;
    cin >> n;
    
    HullPt *P = new HullPt[n];  // input
    for(i=0;i<n;i++){cin>>P[i].x;cin>>P[i].y;cin>>P[i].z;}
    
    HullPt *list = sort(P, n);
    HullPt **A = new HullPt *[2*n], **B = new HullPt *[2*n];
    hull(list, n, A, B);
    
    for (i = 0; A[i] != hNIL; A[i++]->act())  // output
        cout << A[i]->prev-P << " " << A[i]-P << " " << A[i]->next-P << "\n";
    delete A;  delete B;  delete P;
    retrun 0;
}