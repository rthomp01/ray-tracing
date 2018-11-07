#include <iostream>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "lambertian.h"
#include "metal.h"

vec3 color(const ray& r, hitable *world, int depth)
{
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main()
{
	srand(static_cast <unsigned> (time(0)));

	std::ofstream outputFile;
	outputFile.open("output.ppm");

	int nx = 200;
	int ny = 100;
	int ns = 100;

	outputFile << "P3\n" << nx << " " << ny << "\n255\n";

	camera cam(vec3(-2, 2, 1), vec3(0, 0, -1), vec3(0, 1, 0), 90, float(nx) / float(ny));

	hitable *list[4];

	float R = cos(3.14 / 4);
	list[0] = new sphere(vec3(-R, 0, -1), R, new lambertian(vec3(0, 0, 1)));
	list[1] = new sphere(vec3(R, 0, -1), R, new lambertian(vec3(1, 0, 0)));
	list[2] = new sphere(vec3(R, 0, -1), R, new lambertian(vec3(1, 0, 0)));
	list[3] = new sphere(vec3(R, 0, -1), R, new lambertian(vec3(1, 0, 0)));

	hitable *world = new hitable_list(list, 4);

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + random_value()) / float(nx);
				float v = float(j + random_value()) / float(ny);
				ray r = cam.get_ray(u, v);
				//vec3 p = r.point_at_parameter(2.0);
				col += color(r, world, 0);
			}
			
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			outputFile << ir << " " << ig << " " << ib << "\n";
		}
	}

	outputFile.close();
}