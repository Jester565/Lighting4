#pragma once
#include <thread>
#include <list>
#include <mutex>

namespace lighting
{
	class LightSource;
	
	/// <summary>
	/// Manages a seperate thread to process <see cref="LightSource"/>s.
	/// </summary>
	class LightRunnable
	{
	public:		
		/// <summary>
		/// Initializes a new instance of the <see cref="LightRunnable"/> class. Providing a <see cref="LightSource"/> so the thread has work to do.
		/// </summary>
		/// <param name="initialLightSource">The first <see cref="LightBlocker"/> to be added to the <see cref="lightBlockers"/>..</param>
		LightRunnable(LightSource* initialLightSource);
		
		/// <summary>
		/// Inifinite while loop to handle the processing of shadows, called by the <see cref="runThread"/> which is detached in the constructor.
		/// </summary>
		void run();
		
		/// <summary>
		/// Adds a <param to the <see cref="lightSources"/> list for processing.  Returns the list iterator to the element that was added.
		/// </summary>
		/// <param name="lightSource">The <see cref="LightSource"/> to be added.</param>
		/// <returns>Iterator for <see cref="lightSources"/> list where the <paramref name="lightSource"/> was added.</returns>
		std::list <LightSource*>::iterator addLightSource(LightSource* lightSource);

		/// <summary>
		/// Removes a <see cref="LightSource"/> from <see cref="lightSources"/> at the iterator.
		/// </summary>
		/// <param name="removeIter">The iterator pointing to the <see cref="LightSource"/> to remove.</param>
		void removeLightSource(std::list <LightSource*>::iterator removeIter);
		
		/// <summary>
		/// When elements of <see cref="lightBlockers"/> copy data of <see cref="LightBlocker"/>s in the method <see cref="run()"/>, this is set to <code>true</code>.  Set to <code>false</code> when <see cref="LightLayer::detach()"/> is called.
		/// </summary>
		bool lightBlockersCopied;
		
		/// <summary>
		/// Mutex to lock access to <see cref="lightBlockersCopied"/>
		/// </summary>
		std::mutex lightBlockersCopiedMutex;
		
		/// <summary>
		/// When <see cref="run()"/> has finished calling <see cref="LightSource::mapShadePoints()"/> on each element in <see cref="lightBlockers"/>, this is set to <code>true</code>.  Set to <code>false</code> when <see cref="LightLayer::detach()"/> is called.
		/// </summary>
		bool shadowsProcessed;
		
		/// <summary>
		/// Mutex to lock access to <see cref="shadowsProcessedMutex"/>
		/// </summary>
		std::mutex shadowsProcessedMutex;

		std::mutex shadowsProcessedMutex;
		
		/// <summary>
		/// Accessor for the <see cref="runTime"/> attribute.
		/// </summary>
		/// <returns>The amount of time it takes to process all the <see cref="lightBlockers"/></returns>
		unsigned int getRunTime()
		{
			return runTime;
		}
		
		/// <summary>
		/// Finalizes an instance of the <see cref="LightRunnable"/> class.
		/// </summary>
		~LightRunnable();

	private:
		
		/// <summary>
		/// Calls method <see cref="LightSource::createShadePoints()"/> on each element of <see cref="lightBlockers"/>.
		/// </summary>
		void transferLightBlockers();
		
		/// <summary>
		/// Calls method <see cref="LightSource::mapShadePoints()"/> on each element of <see cref="lightBlockers"/>.
		/// </summary>
		void processShadows();

		/// <summary>
		/// Holds all of the <see cref="LightSource"/>s to be processed by <see cref="run()"/>.
		/// </summary>
		std::list <LightSource*> lightSources;
		
		/// <summary>
		/// Thread which calls the method <see cref="run"/>.  Detached and initialized in the consturctor.
		/// </summary>
		std::thread* runThread;
		
		/// <summary>
		/// The amount of milliseconds it takes to process all of the shadows on all <see cref="lightBlockers"/>.
		/// </summary>
		unsigned int runTime;
	};
}
