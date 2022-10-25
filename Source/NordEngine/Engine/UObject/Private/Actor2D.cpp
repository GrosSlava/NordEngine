
#include "Actor2D.h"





AActor2D::AActor2D()
{
}

AActor2D::~AActor2D()
{
}





void AActor2D::PreConstruct()
{
	//TODO for components
}

void AActor2D::OnConstruct()
{
	//TODO for components
}

void AActor2D::BeginPlay()
{
	//TODO for components
}



void AActor2D::Tick(double DeltaTime)
{
	//TODO for components
}

void AActor2D::Destroy()
{
	IsPendingToKill = true;

	//TODO for components
}

