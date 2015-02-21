int HML_PendulumOfMaxwell(double *Data)
{
/*
Итерационная модель маятника Максвелла с затухающими колебаниями с управлением.
Вызов данной процедуры прогоняет только одну итерацию, то есть одно
наблюдение. Последующий вызов процедуры покажет положение маятника в следующий
момент времени. Самим параметры менять не надо при вызове следующей итерации,
так как процедура сама все поменяет (кроме, естественно, управляющего ускорения).
Входные параметры:
 Data - параметры маятника
  [0] - текущее положение маятника. Сюда запишется положение маятника на
  следующей итерации.
  [1] - текущая скорость маятника. Сюда запишется скорость маятника на
  следующей итерации.
  [2] - текущее значение управляющего ускорения. Если равно 0, то управления нет.
  [3] - Сюда запишется значение ускорения маятника на данной итерации.
  [4] - Сюда запишется, сколько на данной итерации произошло неупругих соударений.
  [5] - масса диска.
  [6] - масса оси.
  [7] - радиус диска.
  [8] - радиус оси.
  [9] - длина нити.
  [10] - коэффициент затухания.
  [11] - интервал наблюдений.
  [12] - минимальная амплитуда отскока в нижней точке. Критерий остановки.
Возвращаемое значение:
 1 - все хорошо
 0 - введены недопустимые данные
*/
//из массива данных определяем параметры маятника
    double x=Data[0];
    double v=Data[1];
    double w=Data[2];
    double m=Data[5];
    double maxis=Data[6];
    double R=Data[7];
    double r=Data[8];
    double l=Data[9];
    double k=Data[10];
    double dt=Data[11];
    double e=Data[12];
    //проверка данных
    if (x>l) return 0;
    if (x<R) return 0;
    if (m<=0) return 0;
    if (maxis<0) return 0;
    if (R<=0) return 0;
    if (r<=0) return 0;
    if (r>R) return 0;
    if (l<=R) return 0;
    if (k<0) return 0;
    if (k>=1.) return 0;
    if (dt<=0) return 0;
    if (e<0) return 0;
    //конец проверки данных
    try
    {
        double a,xnew,vnew,xpart,t1,t3,D1,D2;
        bool P1=true,P2=true;
        a=(m*r*r*(HML_G-w))/(0.5*(m*R*R+maxis*r*r)+(m+maxis)*r*r);//текущее ускорение
        if (((x==R)&&(v<0))||((x==l)&&(v>0))) v=-v*(1.-k);//если маятник находится в крайних точках,
        //скорость меняет свой знак
        int i=0,iv=0;
        while ((P1==true)||(P2==true))
        {
            //за промежуток времени маятник может совершить много колебаний. Они просчитываются все
            P1=true;
            P2=true;
            i++;
            //просчитываем, "долетит" маятник до крайних точек за оставшееся время
            D1=v*v-2*a*(x-l);
            D2=v*v-2*a*(x-R);
            if (D1<0) P1=false;
            if (D2<0) P2=false;
            if (P1==true)
            {
                if (a!=0)
                    t1=(-v+sqrt(D1))/a;
                else
                    if (v!=0)
                        t1=(l-x)/v;
                    else
                        t1=-1;
                if (!((t1>0)&&(t1<dt))) P1=false;
            }
            if (P2==true)
            {
                if (a!=0)
                    t3=(-v-sqrt(D2))/a;
                else
                    if (v!=0)
                        t3=(R-x)/v;
                    else
                        t3=-1;
                if (!((t3>0)&&(t3<dt))) P2=false;
            }
            if ((P1==true)&&(P2==false))
            {
                if (x==l)
                {
                    xpart=fabs(v*(t1/2.)+0.5*a*(t1/2.)*(t1/2.));
                    if (xpart<e) v=0;
                }
                x=l;
                v=v+a*t1;
                v=-v*(1.-k);
                dt=dt-t1;
            }
            if ((P2==true)&&(P1==false))
            {
                x=R;
                v=v+a*t3;
                v=-v*(1.-k);
                dt=dt-t3;
                iv++;
            }
        }
        xnew=x+v*dt+0.5*dt*dt*a;//новое положение маятника
        vnew=v+a*dt;//новая скорость маятника
        if ((xnew>l-e)&&(iv==0))//не остановился ли маятник
        {
            xnew=l;
            vnew=0.;
        }
        //проверка на всякий случай, чтобы маятник не улетел за границы дозволенного
        if (xnew<R) xnew=R;
        if (xnew>l) xnew=l;
        //обновляем входные данные массива
        Data[0]=xnew;
        Data[1]=vnew;
        Data[3]=a;
        Data[4]=i-1;
        //если всё прошло хорошо, то возвращается 1
        return 1;
    }
    catch(...)
    {
        //если что-то пошло не так, то возвращается 0
        return 0;
    }
}