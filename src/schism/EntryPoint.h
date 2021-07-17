#pragma once

#ifdef SCHISM_PLATFORM_WINDOWS

extern std::unique_ptr<Schism::Application> Schism::CreateApplication();

int main(int argc, char** argv)
{
	
}

#endif