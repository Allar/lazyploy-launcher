#include "AllarBuilderClientApp.h"
#include "AllarBuilderClient.h"
#include "UniquePtr.h"

#include "TaskGraphInterfaces.h"

FAllarBuilderClient::FAllarBuilderClient()
{
}


FAllarBuilderClient::~FAllarBuilderClient()
{
}

void FAllarBuilderClient::RequestCloseWindow(const TSharedRef<SWindow>& Window)
{
}

FString FAllarBuilderClient::GetProjectPath() const
{
	return ProjectPath;
}

void FAllarBuilderClient::SetProjectPath(const FString NewProjectPath)
{
	ProjectPath = NewProjectPath;
}

FString FAllarBuilderClient::GetProjectName() const
{
	if (!ProjectPath.IsEmpty())
	{
		return FPaths::GetBaseFilename(ProjectPath);
	}
	return FString();
}
