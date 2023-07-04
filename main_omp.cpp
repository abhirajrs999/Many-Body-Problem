#include<iostream>
#include<fstream>
#include<math.h>
#include<vector>
#include<stdlib.h>
#include<omp.h>

#define n_bodies 1000
#define dim_x 100
#define dim_y 200
#define dim_z 400
#define del_t 0.01
#define threads 8
#define NUM_STEPS 720000
# define M 1
# define diameter 1

using namespace std;

class Body
{
public:
	int index;
	float x, y, z;
	float fx, fy, fz;
	float vx, vy, vz;
public:
	void initialize(float, float, float, int);
	void find_force(Body*);
	void half_vel();
	void update_pos(Body *);
	void full_vel();
};

void Body::initialize(float a, float b, float c, int ind)
{
	fx = fy = fx = vx = vy = vz = 0.0;
	x = a;
	y = b;
	z = c;
	index = ind;
}
void readfile(const char* filename, Body* B)
{
	fstream fin(filename);
	string line;
  	for(int i = 0 ; i < 8 ; i++)
  	{
    		getline(fin, line);
  	}

	double rx, ry, rz;
	
	for(int i = 0; i < n_bodies; i++)
	{
		fin>>rx>>ry>>rz;
		B[i].initialize(rx, ry, rz, i);
	}
  
}
float euclidean(double x1, double y1, double z1, double x2, double y2, double z2)
{

	return (pow((x1-x2), 2) + pow((y1-y2), 2) + pow((z1-z2), 2));
}
void Body::find_force(Body *B)
{
	float d;
	#pragma omp parallel for num_threads(threads) shared(B)
	for(int i = 0; i < n_bodies; i++)
	{
		if(i != index)
		{
			d = pow(euclidean(B[i].x, B[i].y, B[i].z, x, y, z),2);
			fx += (B[i].x - x)/d;
			fy += (B[i].y - y)/d;
			fz += (B[i].z - z)/d;
		}
	}
	fx = M*M*fx;
	fy = M*M*fy;
	fz = M*M*fz;
}
void Body::half_vel()
{
	vx = vx + fx*del_t/(2*M);
	vy = vy + fy*del_t/(2*M);
	vz = vz + fz*del_t/(2*M);
}

void Body::update_pos(Body *B)
{
  x = x + vx*del_t;
  y = y + vy*del_t;
  z = z + vz*del_t;
  
  // Checking for boundary conditions and then swapping velocities as it collides with wall
  
  #pragma omp critical
  {
     if(x>= dim_x || x<=0)
     {
        x= (x<=0)?(-x): (2*dim_x - x);
        vx = -vx;
     }
     if(y>= dim_y || y<=0)
     {
        y= (y<=0)?(-y): (2*dim_y - y);
        vy = -vy;
     }
     if(z>= dim_z || z<=0)
     {
        z= (z<=0)?(-z): (2*dim_z - z);
        vz = -vz;
     }
  }
  int idx;
  // checkcing for collision
  #pragma omp parallel for private(idx) shared(B) schedule(dynamic)
  for(idx = 0; idx < n_bodies; idx++)
 
	{
	        float temp_hvx;
	        float temp_hvy;
	        float temp_hvz;
	        float min_dis = diameter;
	        
	         
	       
	        
		if(idx!= index && euclidean(B[idx].x, B[idx].y, B[idx].z, x, y, z)<= min_dis)
		{
		       //swapping of velocity after elastic collision
			temp_hvx = vx;
			vx = B[idx].vx;
			B[idx].vx = temp_hvx;
			
			temp_hvy = vy;
			vy = B[idx].vy;
			B[idx].vy = temp_hvy;
			
			temp_hvz = vz;
			vz = B[idx].vz;
			B[idx].vz = temp_hvz;
			
		}
      }   
	

}

void Body::full_vel()
{
	vx = vx + fx*del_t/2;
	vy = vy + fy*del_t/2;
	vz = vz + fz*del_t/2;
}

void generate_bin_file(Body *B, int step)
{
    	fstream file;
    	string s = "trajectory"+to_string(step)+".bin";
	file.open(s,ios::out|ios::binary);
	int i;
	for(i=0 ; i < n_bodies ; i++)
	{
        	file << B[i].x << " ";
        	file << B[i].y << " ";
        	file << B[i].z <<  " ";
        	file <<"\n";
	}
	file.close();
}

int main()
{
	Body *bodies = new Body[n_bodies];
	readfile("Trajectory.txt", bodies);

	ofstream fout("out.txt");

	double tot_time = 0, wt;

	for(int step = 0; step < NUM_STEPS; step++)
	{
		if(step % 100 == 0) 
			wt = omp_get_wtime();
		
		double wtime = omp_get_wtime();
		#pragma omp parallel for num_threads(threads) shared(bodies)
		for(int i = 0; i < n_bodies; i++)
		{
			bodies[i].find_force(bodies);
			bodies[i].half_vel();
			bodies[i].update_pos(bodies);
			bodies[i].full_vel();
		}
		wtime = omp_get_wtime() - wtime;
		tot_time += wtime; 
		if(step % 100 == 0)
		{
			fout << "Step "<< step <<" took "<< (omp_get_wtime()-wt) << "s \n";
			generate_bin_file(bodies, step);
		}
		//cout<<"Step : "<<step<<" Time : "<<wtime<<" Total Time: "<<tot_time<<endl;
	}
	cout << "Total Iteration Time : " << tot_time << "\n";
	
	fout.close();
	
	/*cout<<bodies[0].x<<endl;
	cout<<bodies[0].vx<<endl;*/
}
