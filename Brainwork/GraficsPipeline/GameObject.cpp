#include "GameObject.h"
#include "VK_Renderer.h"
#include "VK_Device.h"
#include "Camera.h"


GameObject::GameObject(VK_Renderer& p_renderer)
{
	m_renderer = &p_renderer;

	m_BufferObject.SetRenderer(m_renderer);
}

GameObject::GameObject(const GameObject& p_gameObject)
{
	globalMatrix = p_gameObject.globalMatrix;
	localMatrix = p_gameObject.localMatrix;
	m_parentObject = p_gameObject.m_parentObject;
	m_children = p_gameObject.m_children;

	m_renderer = p_gameObject.m_renderer;
	m_object = p_gameObject.m_object;
	m_BufferObject = p_gameObject.m_BufferObject;
	m_material = p_gameObject.m_material;
}

GameObject::~GameObject()
{
}


void GameObject::UpdateGameObject(uint32_t currentImage) {
	
	//localMatrix.rotation3DAroundY(0.05f);

	m_ubo.model = this->getGlobalMatrix();
	m_ubo.view = Camera::ViewCamera.GetCameraMatrix();
	m_ubo.proj.perspectivProjection((WIDTH < HEIGHT) ? (float)WIDTH / (float)HEIGHT : 1, (HEIGHT < WIDTH) ? (float)HEIGHT / (float)WIDTH : 1, 1, 60);
	m_ubo.proj[1][1] *= -1;
	m_ubo.lightPos = Vector4(10.0f, 10.0f, 10.0f, 0.0f);
	m_ubo.lightColor = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
	m_ubo.groundColor = Vector4(0.8f, 0.6f, 0.6f, 0.0f);


	m_BufferObject.UpdateUniformBuffer(m_ubo, currentImage);
}

UniformBufferObject GameObject::GetUniformBufferObject()
{
	return m_ubo;
}

UniformBufferObject& GameObject::GetUniformBufferObjectRef()
{
	return m_ubo;
}

void GameObject::SetObject(const Object p_object)
{
	m_object = p_object;
}

void GameObject::SetMaterial(const Material p_material)
{
	m_material = p_material;
}

void GameObject::SetMaterial()
{
	m_material = Material();
}

VK_BufferObject GameObject::GetBufferObject()
{
	return m_BufferObject;
}

VK_BufferObject & GameObject::GetBufferObjectRef()
{
	return m_BufferObject;
}

Object GameObject::GetObject()
{
	return m_object;
}

Object & GameObject::GetObjectRef()
{
	return m_object;
}

Material GameObject::GetMaterial()
{
	return m_material;
}

Material & GameObject::GetMaterialRef()
{
	return m_material;
}

void GameObject::CreateBuffer()
{
	m_material.GetTextureRef().CreateTexture(m_renderer);

	m_BufferObject.CreateVertexBuffer(m_object.GetVertices());
	m_BufferObject.CreateIndexBuffer(m_object.GetIndices());
	m_BufferObject.CreateUniformBuffers();
}

void GameObject::CreateDescriptorSets()
{
	m_BufferObject.CreateDescriptorSet(m_material);
}


Matrix<float, 4, 4> GameObject::recalculateMatrix()
{
	GameObject* parent = m_parentObject;

	while (parent != NULL)
	{
		globalMatrix *= parent->globalMatrix;
		parent = parent->m_parentObject;
	}

	for (GameObject& child : m_children)
	{
		child.recalculateMatrix();
	}

	return globalMatrix;
}

Matrix<float, 4, 4> GameObject::getGlobalMatrix()
{
	return (globalMatrix * localMatrix).transpose();
}

void GameObject::setParent(GameObject* p_parentObject)
{
	m_parentObject = p_parentObject;
	p_parentObject->addChild(*this);
	this->recalculateMatrix();
}

void GameObject::addChild(GameObject& p_childObject)
{
	p_childObject.setParent(this);
	p_childObject.recalculateMatrix();
	m_children.push_back(p_childObject);
}

void GameObject::addChildren(std::vector<GameObject>& p_childrenObject)
{
	for (GameObject& child : p_childrenObject)
	{
		child.setParent(this);
		child.recalculateMatrix();
		m_children.push_back(child);
	}
}


GameObject* GameObject::getParent()
{
	return m_parentObject;
}

GameObject& GameObject::getChild(uint16_t p_index)
{
	return  m_children[p_index];
}

std::vector<GameObject>& GameObject::getChildren()
{
	return  m_children;
}

void GameObject::CleanupGameObject()
{
	this->GetBufferObject().CleanupBufferObject();
}

