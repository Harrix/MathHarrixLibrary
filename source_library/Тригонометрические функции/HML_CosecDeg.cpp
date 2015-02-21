double HML_CosecDeg(double x)
{
/*
Функция возвращает косеканс угла в градусах.
Входные параметры:
 x - угол в градусах.
Возвращаемое значение:
 Косеканс угла.
*/
return 1./sin(HML_DegToRad(x));
}