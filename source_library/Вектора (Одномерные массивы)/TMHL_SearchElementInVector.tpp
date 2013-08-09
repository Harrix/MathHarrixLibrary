template <class T> int TMHL_SearchElementInVector (T *X, T x, int VMHL_N)
/*
Функция находит номер первого элемента в массиве, равного данному.
Входные параметры:
 X - исходный массив;
 x - данный элемент;
 VMHL_N - размер массива.
Возвращаемое значение:
 Номер элемента.
Примечание:
 Если такого элемента нет, то возвращает -1.
*/
{
int i=0;

while (i<VMHL_N)
	{
	if (X[i]==x) return (i);//нашли элемент
	i++;
	}
	
return -1;//не нашли
}