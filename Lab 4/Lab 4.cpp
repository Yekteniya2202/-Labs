// Lab 3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <omp.h>
#include <cmath>
#include <functional>
using namespace std;

double my_function(double x) {
    return sin(1. / 3.) * log(0.1 + x * x);
}

double calcl_step(double a, double b, int num_steps) {
    if (b < a) throw exception("b must be > a");
    return (b - a) / (double)num_steps;
}

bool check_accuracy(double In, double I2n, double epsilon) {
    return fabs(In - I2n) <= epsilon;
}

double calc_integral(double a, double b, double epsilon, function<double(double, double, int)>calc_method){
    int starting_sum_steps = 1;
    double In, I2n;
    int count = 0;
    do {
        count++;
        In = calc_method(a, b, starting_sum_steps);
        I2n = calc_method(a, b, starting_sum_steps*=2);
    } while (!check_accuracy(In, I2n, epsilon));
    cout << "Число итераций = " << count << endl;
    return I2n;
}

int main()
{
    cout.precision(7);
    omp_set_dynamic(0);
    omp_set_num_threads(8);
    srand(time(NULL));
    setlocale(LC_ALL, "rus");
    double a = 1, b = 3;
    double eps1 = 0.00001, eps2 = 0.000001, eps3 = 0.0000001;
    double res, start, delta;
    function<double(double, double, int)> medium_rect_method = [](double a, double b, int num_steps) -> double {
        double step = calcl_step(a, b, num_steps);
        double x1, x2, sum = 0.0;
        #pragma omp parallel for private(x1, x2) reduction(+:sum)
            for (int i = 0; i < num_steps; i++) {
                x1 = a + (i - 1) * step;
                x2 = a + i * step;
                sum += my_function((x1 + x2)/2);
            }
        return step * sum;
    };

    start = omp_get_wtime();
    res = calc_integral(a, b, eps1, medium_rect_method);
    delta = omp_get_wtime() - start;
    cout << "Метод средних прямоугольников = " << fixed << res << " при epsilon = " << eps1 << " -> " << delta << endl;

    start = omp_get_wtime();
    res = calc_integral(a,b,eps2, medium_rect_method);
    delta = omp_get_wtime() - start;
    cout << "Метод средних прямоугольников = " << fixed << res << " при epsilon = " << eps2 << " -> " << delta << endl;

    start = omp_get_wtime();
    res = calc_integral(a,b,eps3, medium_rect_method);
    delta = omp_get_wtime() - start;
    cout << "Метод средних прямоугольников = " << fixed << res << " при epsilon = " << eps3 << " -> " << delta << endl;

    cout << "==================================================" << endl;

    function<double(double, double, int)> left_rect_method = [](double a, double b, int num_steps) -> double {
        double step = calcl_step(a, b, num_steps);
        double x, sum = 0.0;
        #pragma omp parallel for private(x) reduction(+:sum)
            for (int i = 0; i < num_steps; i++) {
                x = a + (i - 1) * step;
                sum += my_function(x);
            }
        return step * sum;
    };
    start = omp_get_wtime();
    res = calc_integral(a, b, eps1, left_rect_method);
    delta = omp_get_wtime() - start;
    cout << "Метод левых прямоугольников   = " << fixed << res << " при epsilon = " << eps1 << " -> " << delta << endl;

    start = omp_get_wtime();
    res = calc_integral(a, b, eps2, left_rect_method);
    delta = omp_get_wtime() - start;
    cout << "Метод левых прямоугольников   = " << fixed << res << " при epsilon = " << eps2 << " -> " << delta << endl;

    start = omp_get_wtime();
    res = calc_integral(a, b, eps3, left_rect_method);
    delta = omp_get_wtime() - start;
    cout << "Метод левых прямоугольников   = " << fixed << res << " при epsilon = " << eps3 << " -> " << delta << endl;

    cout << "==================================================" << endl;

    function<double(double, double, int)> right_rect_method = [](double a, double b, int num_steps) -> double {
        double step = calcl_step(a, b, num_steps);
        double x, sum = 0.0;
        #pragma omp parallel for private(x) reduction(+:sum)
            for (int i = 0; i < num_steps; i++) {
                x = a + i * step;
                sum += my_function(x);
            }
        return step * sum;
    };

    start = omp_get_wtime();
    res = calc_integral(a, b, eps1, right_rect_method);
    delta = omp_get_wtime() - start;
    cout << "Метод правых прямоугольников  = " << fixed << res << " при epsilon = " << eps1 << " -> " << delta << endl;

    start = omp_get_wtime();
    res = calc_integral(a, b, eps2, right_rect_method);
    delta = omp_get_wtime() - start;
    cout << "Метод правых прямоугольников  = " << fixed << res << " при epsilon = " << eps2 << " -> " << delta << endl;

    start = omp_get_wtime();
    res = calc_integral(a, b, eps3, right_rect_method);
    delta = omp_get_wtime() - start;
    cout << "Метод правых прямоугольников  = " << fixed << res << " при epsilon = " << eps3 << " -> " << delta << endl;

    cout << "==================================================" << endl;

    function<double(double, double, int)> trapezoid_method = [](double a, double b, int num_steps) -> double {
        double step = calcl_step(a, b, num_steps);
        double x1, x2, sum = 0.0;
        #pragma omp parallel for private(x1, x2) reduction(+:sum)
            for (int i = 0; i < num_steps; i++) {
                x1 = a + (i - 1) * step;
                x2 = a + i * step;
                sum += (my_function(x1) + my_function(x2)) / 2 * step;
            }
        return sum;
    };
    start = omp_get_wtime();
    res = calc_integral(a, b, eps1, trapezoid_method);
    delta = omp_get_wtime() - start;
    cout << "Метод трапеций                = " << fixed << res << " при epsilon = " << eps1 << " -> " << delta << endl;

    start = omp_get_wtime();
    res = calc_integral(a,b,eps2, trapezoid_method);
    delta = omp_get_wtime() - start;
    cout << "Метод трапеций                = " << fixed << res << " при epsilon = " << eps2 << " -> " << delta << endl;

    start = omp_get_wtime();
    res = calc_integral(a, b, eps3, trapezoid_method);
    delta = omp_get_wtime() - start;
    cout << "Метод трапеций                = " << fixed << res << " при epsilon = " << eps3 << " -> " << delta << endl;

    cout << "==================================================" << endl;

    function<double(double, double, int)> simpson_method = [](double a, double b, int num_steps) -> double {
        double step = calcl_step(a, b, num_steps);
        double x1, x2, x, sum = 0.0;
        #pragma omp parallel for private(x1, x2, x) reduction(+:sum)
            for (int i = 0; i < num_steps; i++) {
                x1 = a + (i - 1) * step;
                x = a + (i - 0.5) * step;
                x2 = a + i * step;
                sum += my_function(x1) + 4 * my_function(x) + my_function(x2);
            }
        return step / 6. * sum;
    };
    start = omp_get_wtime();
    res = calc_integral(a, b, eps1, simpson_method);
    delta = omp_get_wtime() - start;
    cout << "Метод Симпсона                = " << fixed << res << " при epsilon = " << eps1 << " -> " << delta << endl;

    start = omp_get_wtime();
    res = calc_integral(a, b, eps2, simpson_method);
    delta = omp_get_wtime() - start;
    cout << "Метод Симпсона                = " << fixed << res << " при epsilon = " << eps2 << " -> " << delta << endl;

    start = omp_get_wtime();
    res = calc_integral(a, b, eps3, simpson_method);
    delta = omp_get_wtime() - start;
    cout << "Метод Симпсона                = " << fixed << res << " при epsilon = " << eps3 << " -> " << delta << endl;

    system("pause");
}