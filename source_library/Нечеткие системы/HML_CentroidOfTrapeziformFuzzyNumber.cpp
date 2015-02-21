double HML_CentroidOfTrapeziformFuzzyNumber(double a,double b,double c,double d)
{
/*
Определяет центр тяжести трапециевидного нечеткого числа.
Входные параметры:
 a - левая крайняя граница.
 b - начало устойчивой области.
 с - конец устойчивой области.
 d - правая крайняя граница.
Возвращаемое значение:
 Центр тяжести трапециевидного числа.
*/
    double f,f1,f2;
    f1=a*a+b*b-c*c-d*d+b*a-c*d;
    f2=a+b-c-d;
    if (f2==0)
        f=(d-a)/2.;
    else
        f=f1/(3.*f2);
    return f;
}