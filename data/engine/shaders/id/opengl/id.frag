#version 450 core

out uint FragColor;
uniform uint objectId;

void main(){
	FragColor = objectId;
}