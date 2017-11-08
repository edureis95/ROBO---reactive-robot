#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/Range.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Bool.h>
#include <geometry_msgs/Twist.h>
#include <stdlib.h>
#include <math.h>

ros:: Publisher pub;

float dist_sonar0;
float dist_sonar1;
float dist_sonar2;

int fase = 0;

float vel = 0.4; //velocidade
float d_med = 0.8; //distancia media a parede
float d_min = 0.7; //distancia minima a parede
float d_max = 0.9; //distancia maxima a parede
float rad = 0.40; //angulo de rotacao


void sonarCallback_sonar_1(const sensor_msgs::Range::ConstPtr& m)
{
	dist_sonar1 = m->range;
	//ROS_INFO_STREAM("\n dist_sonar1: " << dist_sonar1);
}

void sonarCallback_sonar_2(const sensor_msgs::Range::ConstPtr& m)
{
	dist_sonar2 = m->range;
	ROS_INFO_STREAM("\n dist_sonar2: " << dist_sonar2);
}

void sonarCallback_sonar_0(const sensor_msgs::Range::ConstPtr& m)
{
	geometry_msgs:: Twist msg;

	dist_sonar0 = m->range;
	//ROS_INFO_STREAM("\n dist_sonar0: " << dist_sonar0);

	ros::Rate r(2);

	if(fase == 0){	//procura a parede mais proxima 
		
		if(dist_sonar1 < dist_sonar2 && dist_sonar1 < dist_sonar0){ 
			msg.angular.z = M_PI; //roda 90 se a mais proxima for a da esquerda
		}
		else if(dist_sonar2 < dist_sonar1 && dist_sonar2 < dist_sonar0){
			msg.angular.z = M_PI; //roda -90 se a mais proxima for a da direita
		}
		fase = 1; //nao faz nada se a mais proxima for a da frente ou estejam todas fora do alcance

	}
	else if(fase == 1){ //move-se até à parede mais próxima
		if(dist_sonar0 > d_med){
			msg.linear.x = vel;
		}
		else{
			fase = 2;
		}		
		
	}
	else if(fase == 2){ //circula a parede
				
		if(dist_sonar0 >= d_max){ //boa distancia da parede da frente
			if(d_min < dist_sonar2 && dist_sonar2 <= d_max){ //esta a uma boa distancia da parede do lado
				msg.linear.x = vel;
			}
			else if(dist_sonar2 <= d_min){ 
				if(dist_sonar2 < (d_min-2)){ //pertissimo da parede do lado
					msg.angular.z = rad * 6;
					msg.linear.x = vel;
				}
				else if(dist_sonar2 < (d_min-1)){ //perto de mais da parede do lado
					msg.angular.z = rad * 3;
					msg.linear.x = vel;
				}
				else{ //perto da parede do lado
					//msg.angular.z = rad;
					msg.linear.x = vel;
				}
			}
			else{/*
				ROS_INFO_STREAM("\n entrou ");
				if(dist_sonar2 > (d_max+2)){ //longissimo da parede do lado
					msg.angular.z = -rad*6;
					msg.linear.x = vel;

				}*/
				if(dist_sonar2 > (d_max+1)){ //longe de mais da parede do lado
					msg.angular.z = -rad;
					//msg.linear.x = vel;

				}
				else{ //longe da parede do lado
					msg.angular.z = -rad;
					msg.linear.x = vel;

				}
			}
			
		}
		else if(d_med < dist_sonar0 && dist_sonar0 < d_max){ //perto da parede da frente
			if(dist_sonar2 < d_min){
				msg.angular.z = rad;
			}
			else{
				msg.angular.z = rad;
				msg.linear.x = vel;
			}			

		}
		else{ //pertissimo da parede da frente
			msg.angular.z = rad;
			
		}
		
	}

	pub.publish(msg);
	r.sleep();
}




int main(int argc, char **argv) 
{
	ros::init(argc, argv, "reactive_robot");
    	ros::NodeHandle NodeHandle; 

	ros::Subscriber sonar_subscriber = NodeHandle.subscribe("robot0/sonar_0", 1, sonarCallback_sonar_0);

	ros::Subscriber sonar_subscriber1 = NodeHandle.subscribe("robot0/sonar_1", 1, sonarCallback_sonar_1);
    
	ros::Subscriber sonar_subscriber2 = NodeHandle.subscribe("robot0/sonar_2", 1, sonarCallback_sonar_2);

	pub = NodeHandle.advertise <geometry_msgs:: Twist > ("robot0/cmd_vel" , 1000) ;

    	srand(time(0));

    	ros::spin(); 
    
    	return 0; 
}

