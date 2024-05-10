/*
Proyecto - MAXIMILIANO QUIÑONES REYES 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;
//bandera
bool keyXPressed = false;
bool keyZPressed = false;

bool luz1 = true;
bool luz2 = true;

//vehículo motorizado JUGGERNAUT
float movJuggernautX, movJuggernautZ;
float movJuggernautOffset;
float rotLlantaJuggernaut;
float rotLlantaJuggernautOffset;
float rotVehiculo;
float rotVehiculoOffset;
bool rot1, rot2, rot3, rot4, rot5, rot6, ciclo_J;
bool bandera_J;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

//ShipLuffy
Model ship_luffy;

//ShipLuffy
Model whale_luffy;

//ShipLuffy
Model luffy;

//AceLuffy
Model ace_luffy;


//-----CASA-------
Model casa_Kamino_M;

Texture dirtTexture;
Texture pisoTexture;
Texture aguaTexture;
Texture cespedTexture;


//--TEXTURAS MODELOS------
// 
//Vehículo -Terrestre- Juggernaut
Model juggernaut_M;
Model juggernaut_llanta1_M;
Model juggernaut_llanta2_M;
Model juggernaut_llanta3_M;
Model juggernaut_llanta4_M;
Model juggernaut_llanta5_M;
Model juggernaut_llanta6_M;
Model juggernaut_llanta7_M;
Model juggernaut_llanta8_M;
Model juggernaut_llanta9_M;
Model juggernaut_llanta10_M;
Model juggernaut_torreta_lateral_M;
Model juggernaut_torreta_frontal_M;
Model juggernaut_torreta_trasera_M;

Model casa_cupH_M;
Model cupH_M;

//objetos
Model lampara_M;
Model banca_M;
Model puente_M;
Model girasol_M;
Model arbol_M;
Model centro_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];

SpotLight spotLights[MAX_POINT_LIGHTS];


// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		-1.0f, -1.0f, -1.0f,  //0,0,0
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		-1.0f, -1.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		-1.0f, -1.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		-1.0f, -1.0f, -1.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		-1.0f, -1.0f, -1.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 1.0f, 1.0f);

	//----------------LAMPARA----------------------------
	lampara_M = Model();
	lampara_M.LoadModel("Models/lampara.obj");

	//---------------------KAMINO------------------
	casa_Kamino_M = Model();
	casa_Kamino_M.LoadModel("Models/kamino.obj");

	//---------------------SHIP LUFFY------------------
	ship_luffy = Model();
	ship_luffy.LoadModel("Models/model-ship/merry.obj");

	//---------------------SHIP LUFFY------------------
	whale_luffy = Model();
	whale_luffy.LoadModel("Models/whale/whale2.obj");

	//---------------------SHIP LUFFY------------------
	luffy = Model();
	luffy.LoadModel("Models/luffy/luffy2.obj");

	//---------------------SHIP LUFFY------------------
	ace_luffy = Model();
	ace_luffy.LoadModel("Models/ace/ace.obj");

	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	aguaTexture = Texture("Textures/agua.tga");
	aguaTexture.LoadTextureA();
	cespedTexture = Texture("Textures/cesped.tga");
	cespedTexture.LoadTextureA();

	//----------------JUGGERNAUT----------------------------
	juggernaut_M = Model();
	juggernaut_M.LoadModel("Models/Models-Max/Juggernaut/juggernaut.obj");

	juggernaut_llanta1_M = Model();
	juggernaut_llanta1_M.LoadModel("Models/Models-Max/Juggernaut/juggernaut-llanta1.obj");

	juggernaut_llanta2_M = Model();
	juggernaut_llanta2_M.LoadModel("Models/Models-Max/Juggernaut/juggernaut-llanta2.obj");

	juggernaut_llanta3_M = Model();
	juggernaut_llanta3_M.LoadModel("Models/Models-Max/Juggernaut/juggernaut-llanta3.obj");

	juggernaut_llanta4_M = Model();
	juggernaut_llanta4_M.LoadModel("Models/Models-Max/Juggernaut/juggernaut-llanta4.obj");

	juggernaut_llanta5_M = Model();
	juggernaut_llanta5_M.LoadModel("Models/Models-Max/Juggernaut/juggernaut-llanta5.obj");

	juggernaut_llanta6_M = Model();
	juggernaut_llanta6_M.LoadModel("Models/Models-Max/Juggernaut/juggernaut-llanta6.obj");

	juggernaut_llanta7_M = Model();
	juggernaut_llanta7_M.LoadModel("Models/Models-Max/Juggernaut/juggernaut-llanta7.obj");

	juggernaut_llanta8_M = Model();
	juggernaut_llanta8_M.LoadModel("Models/Models-Max/Juggernaut/juggernaut-llanta8.obj");

	juggernaut_llanta9_M = Model();
	juggernaut_llanta9_M.LoadModel("Models/Models-Max/Juggernaut/juggernaut-llanta9.obj");

	juggernaut_llanta10_M = Model();
	juggernaut_llanta10_M.LoadModel("Models/Models-Max/Juggernaut/juggernaut-llanta10.obj");

	juggernaut_torreta_lateral_M = Model();
	juggernaut_torreta_lateral_M.LoadModel("Models/Models-Max/Juggernaut/juggernaut-torreta-lateral.obj");

	juggernaut_torreta_frontal_M = Model();
	juggernaut_torreta_frontal_M.LoadModel("Models/Models-Max/Juggernaut/juggernaut-torreta-frontal.obj");

	juggernaut_torreta_trasera_M = Model();
	juggernaut_torreta_trasera_M.LoadModel("Models/Models-Max/Juggernaut/juggernaut-torreta-trasera.obj");

	//CUPHEAD
	casa_cupH_M = Model();
	casa_cupH_M.LoadModel("Models/casa_cuphead.obj");

	cupH_M = Model();
	cupH_M.LoadModel("Models/cupheadF.obj");

	//objetos
	puente_M = Model();
	puente_M.LoadModel("Models/puente_M.obj");

	//banca_M = Model();
//banca_M.LoadModel("Models/banca_M.obj");

	arbol_M = Model();
	arbol_M.LoadModel("Models/arbol.obj");

	girasol_M = Model(),
	girasol_M.LoadModel("Models/girasol_M.obj");

	centro_M = Model();
	centro_M.LoadModel("Models/centro.obj");





	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/xochimilco_rt.jpg");
	skyboxFaces.push_back("Textures/Skybox/xochimilco_lf.jpg");
	skyboxFaces.push_back("Textures/Skybox/xochimilco_dn.jpg");
	skyboxFaces.push_back("Textures/Skybox/xochimilco_up.jpg");
	skyboxFaces.push_back("Textures/Skybox/xochimilco_bk.jpg");
	skyboxFaces.push_back("Textures/Skybox/xochimilco_ft.jpg");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);   //0,0,-1

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	// Ligada al modelo de lampara
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.5f, 0.3f,
		0.0f, 0.0f, 0.0f,
		0.2f, 0.01f, 0.001f);
	pointLightCount++;

	//-----------------------------------------------

	unsigned int spotLightCount = 0;
	//linterna
	//*
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	//*/

	//se crean mas luces puntuales y spotlight 
	//...

	//*/

	//------------giro para el vehículo motorizado JUGGERNAUT----------------------
	movJuggernautX = 0.0f;
	movJuggernautZ = 0.0f;
	movJuggernautOffset = 10.0f; //velocidad del vehiculo
	rotLlantaJuggernaut = 0.0f;
	rotLlantaJuggernautOffset = 10.0f;

	rotVehiculo = 0.0;
	rotVehiculoOffset = 1.0f;

	rot1 = false;
	rot2 = false;
	rot3 = false;
	rot4 = false;
	rot5 = false;
	rot6 = false;
	ciclo_J = false;
	bandera_J = false;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 model_piso(1.0);
	glm::mat4 model_lamp(1.0);
	glm::mat4 model_sable(1.0);
	glm::mat4 model_juggernaut(1.0);
	glm::mat4 model_juggernaut_art1(1.0);
	glm::mat4 model_juggernaut_art2(1.0);
	glm::mat4 model_juggernaut_art3(1.0);
	glm::mat4 model_juggernaut_art4(1.0);
	glm::mat4 model_juggernaut_art5(1.0);
	glm::mat4 model_juggernaut_art6(1.0);
	glm::mat4 model_juggernaut_art7(1.0);
	glm::mat4 model_juggernaut_art8(1.0);
	glm::mat4 model_juggernaut_art9(1.0);
	glm::mat4 model_juggernaut_art10(1.0);
	glm::mat4 model_juggernaut_art11(1.0);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());   //comentar y ver el resultado
		
		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);

		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		//------------------------------PISO--------------------------------
		//*
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		
		aguaTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(600.0f, -1.0f, 0.0f));
		model_piso = model;
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		cespedTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//glBindTexture(GL_TEXTURE_2D, 0); // Desactivar textura
		//*/
		//-------------------------KAMINO-------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 50.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		casa_Kamino_M.RenderModel();

		//-------------------------Ace - LUFFY-------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(550.0f, -5.0f, 100.0f));
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		ace_luffy.RenderModel();


		//-------------------------SHIPLUFFY-------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, -25.0f, 150.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		whale_luffy.RenderModel();

		//-------------------------SHIPLUFFY-------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 40.0f, 150.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		ship_luffy.RenderModel();


		//-------------------------SHIPLUFFY-------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 65.0f, 200.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		luffy.RenderModel();


		//----------------LAMPARA----------------------------
		if (true) {
			model = model_piso;
			model = glm::translate(model, glm::vec3(5.0f, 0.0f, 6.0));
			model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
			model_lamp = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			lampara_M.RenderModel();

			// Obtener la posición de la lampara después de renderizarla
			glm::vec3 lampPos = glm::vec3(model[3][0] + 0.0f, model[3][1] + 22.0f, model[3][2] + 0.0f); // <- con esto hacemos que la luz siga al modelo
			// Configurar la posición y la dirección de la luz
			pointLights[0].SetPosition(lampPos);
		}

		//----------------VEHÍCULO MOTORIZADO -> JUGGERNAUT----------------------------
		if (true) {
			//Cabina principal
			model = model_piso;
			model = glm::translate(model, glm::vec3(-50.0f, 4.5f, -40.0));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			model = glm::translate(model, glm::vec3(movJuggernautX, 0.0f, movJuggernautZ));
			model = glm::rotate(model, rotVehiculo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model_juggernaut = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			juggernaut_M.RenderModel();

			//Llantas izquierdas
			model = model_juggernaut;
			model = glm::translate(model, glm::vec3(-150.0f, -5.5f, 0.0));
			model_juggernaut_art1 = model;
			model = glm::rotate(model, rotLlantaJuggernaut * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			juggernaut_llanta1_M.RenderModel();

			model = model_juggernaut_art1;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -102.0));
			model_juggernaut_art2 = model;
			model = glm::rotate(model, rotLlantaJuggernaut * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			juggernaut_llanta2_M.RenderModel();

			model = model_juggernaut_art2;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -102.0));
			model_juggernaut_art3 = model;
			model = glm::rotate(model, rotLlantaJuggernaut * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			juggernaut_llanta3_M.RenderModel();

			model = model_juggernaut_art3;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -102.0));
			model_juggernaut_art4 = model;
			model = glm::rotate(model, rotLlantaJuggernaut * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			juggernaut_llanta1_M.RenderModel();

			model = model_juggernaut_art4;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -102.0));
			model = glm::rotate(model, rotLlantaJuggernaut * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			juggernaut_llanta2_M.RenderModel();

			//Lantas derechas
			model = model_juggernaut;
			model = glm::translate(model, glm::vec3(150.0f, 0.0f, 0.0));
			model_juggernaut_art5 = model;
			model = glm::rotate(model, rotLlantaJuggernaut * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			juggernaut_llanta6_M.RenderModel();

			model = model_juggernaut_art5;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -102.0));
			model_juggernaut_art6 = model;
			model = glm::rotate(model, rotLlantaJuggernaut * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			juggernaut_llanta7_M.RenderModel();

			model = model_juggernaut_art6;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -102.0));
			model_juggernaut_art7 = model;
			model = glm::rotate(model, rotLlantaJuggernaut * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			juggernaut_llanta8_M.RenderModel();

			model = model_juggernaut_art7;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -102.0));
			model_juggernaut_art8 = model;
			model = glm::rotate(model, rotLlantaJuggernaut * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			juggernaut_llanta6_M.RenderModel();

			model = model_juggernaut_art8;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -102.0));
			model = glm::rotate(model, rotLlantaJuggernaut * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			juggernaut_llanta10_M.RenderModel();

			//Torretas

			model = model_juggernaut;
			model = glm::translate(model, glm::vec3(0.0f, 150.0f, -12.0));
			model_juggernaut_art9 = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			juggernaut_torreta_frontal_M.RenderModel();

			model = model_juggernaut_art9;
			model = glm::translate(model, glm::vec3(1.0f, 10.0f, -247.0));
			model_juggernaut_art10 = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			juggernaut_torreta_trasera_M.RenderModel();

			model = model_juggernaut_art10;
			model = glm::translate(model, glm::vec3(-92.0f, -75.0f, -22.0));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			juggernaut_torreta_lateral_M.RenderModel();

		}


		//------------------------MOVIMIENTO VEHÍCULO MOTORIZADO - JUGGERNAUT--------------------------
		if (true) {
			if (!ciclo_J)
			{
				if (movJuggernautZ < 2950.0f && !bandera_J)
				{
					movJuggernautZ += movJuggernautOffset * deltaTime;
					//printf("avanza%f \n ",movCoche);
					rotLlantaJuggernaut += rotLlantaJuggernautOffset * deltaTime;
					std::cout << "1era PosicionZ: " << movJuggernautZ << std::endl;
					if (movJuggernautZ >= 5000.0f) {
						movJuggernautZ = 0;
					}
					if (movJuggernautZ >= 2945.0f) {
						std::cout << "-----Limite SuperiorZ: " << movJuggernautZ << std::endl;
						bandera_J = true; //Bandera para evitar que se regrese a este punto mas adelante
						rot1 = true;
					}
				}
				else if (rotVehiculo <= 90.0 && rot1) {
					rotVehiculo += rotVehiculoOffset * deltaTime;
					std::cout << "Angulo1: " << rotVehiculo << std::endl;
					if (rotVehiculo >= 90.0) {
						rot1 = false;
					}
				}
				else if (movJuggernautX <= 3000.0f && !rot1 && !rot2) {
					movJuggernautX += movJuggernautOffset * deltaTime;
					rotLlantaJuggernaut += rotLlantaJuggernautOffset * deltaTime;
					std::cout << "PosicionX: " << movJuggernautX << std::endl;
					if (movJuggernautX >= 3000.0f) {
						std::cout << "-----Limite SuperiorX: " << movJuggernautX << std::endl;
						rot2 = true;
					}
				}
				else if (rotVehiculo <= 180.0 && rot2) {
					rotVehiculo += rotVehiculoOffset * deltaTime;
					std::cout << "Angulo2: " << rotVehiculo << std::endl;
					if (rotVehiculo >= 180.0) {
						std::cout << "Exito FINAL 1 ----------------- " << std::endl;
						rot2 = false;
					}
				}
				else if (movJuggernautZ > -2150.0f) {
					movJuggernautZ -= movJuggernautOffset * deltaTime;
					rotLlantaJuggernaut += rotLlantaJuggernautOffset * deltaTime;
					if (movJuggernautZ <= -2145.0f) {
						std::cout << "-----Limite 2 InferiorZ: " << movJuggernautZ << std::endl;
						rot3 = true;
					}
				}
				else if (rotVehiculo >= 0.0 && rot3) {
					rotVehiculo -= rotVehiculoOffset * deltaTime;
					std::cout << "Angulo3: " << rotVehiculo << std::endl;
					if (rotVehiculo <= 0.0) {
						rot3 = false;
						ciclo_J = true;
					}
				}
			}
			else
			{
				if (movJuggernautZ < 2950.0f && bandera_J) {
					movJuggernautZ += movJuggernautOffset * deltaTime;
					rotLlantaJuggernaut += rotLlantaJuggernautOffset * deltaTime;
					std::cout << "2da PosicionZ: " << movJuggernautZ << std::endl;
					if (movJuggernautZ >= 2945.0f) {
						std::cout << "-----Limite 2 SuperiorZ: " << movJuggernautZ << std::endl;
						rot4 = true;
						bandera_J = false;
					}
				}
				else if (rotVehiculo >= -90.0f && rot4) {
					rotVehiculo -= rotVehiculoOffset * deltaTime;
					std::cout << "Angulo4: " << rotVehiculo << std::endl;
					if (rotVehiculo <= -90.0f) {
						rot4 = false;
					}
				}
				else if (movJuggernautX >= 0.0f) {
					movJuggernautX -= movJuggernautOffset * deltaTime;
					rotLlantaJuggernaut += rotLlantaJuggernautOffset * deltaTime;
					std::cout << "PosicionX: " << movJuggernautX << std::endl;
					if (movJuggernautX <= 0.0f) {
						std::cout << "-----Limite 2 InferiorX: " << movJuggernautX << std::endl;
						rot5 = true;
					}
				}
				else if (rotVehiculo >= -180.0 && rot5) {
					rotVehiculo -= rotVehiculoOffset * deltaTime;
					std::cout << "Angulo5: " << rotVehiculo << std::endl;
					if (rotVehiculo <= -180.0) {
						rot5 = false;
					}
				}
				else if (movJuggernautZ > -2150.0f && !bandera_J) {
					movJuggernautZ -= movJuggernautOffset * deltaTime;
					rotLlantaJuggernaut += rotLlantaJuggernautOffset * deltaTime;
					if (movJuggernautZ <= -2145.0f) {
						std::cout << "-----Limite 3 InferiorZ: " << movJuggernautZ << std::endl;
						rot6 = true;
					}
				}
				else if (rotVehiculo <= 0.0 && rot6) {
					rotVehiculo += rotVehiculoOffset * deltaTime;
					std::cout << "Angulo3: " << rotVehiculo << std::endl;
					if (rotVehiculo >= 0.0) {
						rot6 = false;
						ciclo_J = false;
					}
				}
			}
		}


		//__________________________________CUPHEAD________________________________________________________________________

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(250.0f, 10.0f, -199.0f));
		model = glm::scale(model, glm::vec3(0.90f, 0.90f, 0.90f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		casa_cupH_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(310.0f, 15.0f, 10.0f));
		model = glm::scale(model, glm::vec3(3.90f, 3.90f, 3.90f));
		model = glm::rotate(model, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		cupH_M.RenderModel();

// ___________________________OBJETOS_________________
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(280.0f, 10.0f, 10.0f));
		model = glm::scale(model, glm::vec3(.90f, .90f, .90f));
		model = glm::rotate(model, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		puente_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(280.0f, 10.0f, 30.0f));
		model = glm::scale(model, glm::vec3(8.90f, 8.90f, 8.90f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(540.0f, 10.0f, 570.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		centro_M.RenderModel();
	


		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}
