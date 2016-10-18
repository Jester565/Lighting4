#pragma once
#include <unordered_map>
#include <list>
#include <queue>
#include <allegro5/bitmap.h>
#include "LightBlocker.h"

namespace lighting
{
	class LightSource;
	class LightRunnable;

	/// <summary>
	/// The core of the lighting system.  Holds all <see cref="LightSource"/>s and LightBlockers, handles drawing operations, and manages threads.
	/// </summary>
	class LightLayer
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="LightLayer"/> class.  
		/// </summary>
		/// <param name="drawToBmpW">The draw to BMP w.</param>
		/// <param name="drawToBmpH">The draw to BMP h.</param>
		/// <param name="lightBmpScale">The light BMP scale.</param>
		/// <param name="maxThreads">The maximum threads.</param>
		LightLayer(int drawToBmpW, int drawToBmpH, double lightBmpScale, size_t maxThreads);
				
		/// <summary>
		/// Detaches all elements of <see cref="lightRunnables"/> to process <see cref="LightSource"/>s.
		/// Attributes such as location and angle of <see cref="LightSource"/>s and <see cref="LightBlocker"/>s should be set before called
		/// or you will have to wait until the next call to detach before they are implemented.
		/// </summary>
		void detach();
		
		/// <summary>
		/// Draws all of the <see cref="LightSource"/>s to the <see cref="lightMap"/> after all elements in <see cref="lightRunnables"/> have finished
		/// processing shadows.  Gaussian blurs will be applied to the map and everything will be drawn to the display.
		/// </summary>
		void draw();

		/// <summary>
		/// The safe option for adding the <see cref="LightSource"/> object specified by the parameter <paramref name="lightSource"/>.
		/// </summary>
		/// <para>
		/// This method is safe because it will not attempt to add <paramref name="lightSource"/> if <see cref="threadsProcessing"/> is true.
		/// If <see cref="threadsProcessing"/> is true, <paramref name="lightSource"/> is pushed to <see cref="heldAddLightSources"/>.
		/// Otherwise <see cref="addLightSourceUnsafe(LightSource*)"/> is called.
		/// </para>
		/// <param name="lightSource">The <see cref="LightSource"/> to be added.</param>
		void addLightSource(LightSource* lightSource);
		
		/// <summary>
		/// The safe option for removing the <see cref="LightSource"/> object specified by the parameter <paramref name="lightSource"/>.
		/// </summary>
		/// <para>
		/// This method is safe because it will not attempt to remove <paramref name="lightSource"/> if <see cref="threadsProcessing"/> is true.
		/// If <see cref="threadsProcessing"/> is true, <paramref name="lightSource"/> is pushed to <see cref="heldRemoveLightSources"/>.
		/// Otherwise <see cref="removeLightSourceUnsafe(LightSource*)"/> is called.
		/// </para>
		/// <param name="lightSource">The <see cref="LightSource"/> to be removed.</param>
		void removeLightSource(LightSource* lightSource);
		
		/// <summary>
		/// Adds <paramref name="lightBlocker"/> to <see cref="lightBlockers"/> and saves its location in that list in <see cref="lightBlockerTrackerMap"/>.
		/// </summary>
		/// <param name="lightBlocker">The <see cref="LightBlocker"/> to be added.</param>
		void addLightBlocker(LightBlocker* lightBlocker);
		
		/// <summary>
		/// Removes <paramref name="lightBlocker"/> from <see cref="lightBlockers"/> using its location from <see cref="lightBlockerTrackerMap"/>.
		/// </summary>
		/// <param name="lightBlocker">The light blocker.</param>
		void removeLightBlocker(LightBlocker* lightBlocker);
		
		/// <summary>
		/// Finalizes an instance of the <see cref="LightLayer"/>.  None of the <see cref="LightBlocker"/>s or <see cref="LightSource"/> are deleted.  Everything else is destroyed.
		/// </summary>
		~LightLayer();

	private:		
		/// <summary>
		/// Transfers held variables and elements (attributes that can't be modified while processing <see cref="LightSource"/>s)
		/// to their appropiate data members.  Called by <see cref="detach()"/>.
		/// </summary>
		void transferHeldVars();

		/// <summary>
		/// Finds the lightRunnable with the best run time and adds <paramref name="lightSource"/> to it.
		/// Will also add <paramref name="lightSource"/> to <see cref="lightSourceTrackerMap"/>.
		/// </summary>
		/// <param name="lightSource">The <see cref="LightSource"/> to be added.</param>
		void addLightSourceUnsafe(LightSource* lightSource);
		
		/// <summary>
		/// Directly removes <paramref name="lightSource"/> from the <see cref="LightRunnable"/> it is stored in using
		/// the <see cref="lightSourceTrackerMap"/> to find it.
		/// </summary>
		/// <param name="lightSource">The <see cref="LightSource"/> to be removed.</param>
		void removeLightSourceUnsafe(LightSource* lightSource);

		/// <summary>
		/// Sets <see cref="maxThreads"/> to the number cores on the computer.
		/// </summary>
		void setMaxThreadsToNumCores();

		/// <summary>
		/// The <see cref="LightRunnable"/>s used to handle various <see cref="LightSource"/> methods in a seperate thread.
		/// </summary>
		std::list <LightRunnable*> lightRunnables;

		/// <summary>
		/// Temporarily stores <see cref="LightSource"/> objects when they cannot be directly added.  When <see cref="transferHeldVars()"/> is called, the queue is cleared.
		/// </summary>
		std::queue <LightSource>* heldAddLightSources;
		
		/// <summary>
		///  Temporarily stores <see cref="LightSource"/> objects when they cannot be directly removed.  When <see cref="transferHeldVars()"/> is called, the queue is cleared.
		/// </summary>
		std::queue <LightSource*> heldRemoveLightSources;
		
		/// <summary>
		/// Stores all of the <see cref="LightSource"/>s specific location in the <see cref="LightRunnable/"> so they can be quickly removed.
		/// </summary>
		std::unordered_map <LightSource*, std::pair <LightRunnable*, std::list <LightSource*>::iterator>> lightSourceTrackerMap;		
				
		/// <summary>
		/// Stores all of the elements of <see cref="lightBlockers"/> as keys and their location in the list as values to improve access and removal time.
		/// </summary>
		std::unordered_map <LightBlocker*, std::list <LightBlocker*>::iterator> lightBlockerTrackerMap;
				
		/// <summary>
		/// Stores all of the <see cref="LightBlocker"/>s that will be processed by <see cref="LightSource">s.
		/// </summary>
		std::list <LightBlocker*> lightBlockers;

		/// <summary>
		/// The bitmap where all <see cref="LightSource"/>s are drawn to and blurring and blending operations are preformed.  Initialized by the constructor and is not reassigned.
		/// </summary>
		ALLEGRO_BITMAP* lightMap;
		
		/// <summary>
		/// Indicates whether the lightRunnables are processing shadows.  Set to <code>true</code> by <see cref="detach()"/ and set to <code>false</code> by <see cref="draw()"/>.
		/// </summary>
		bool threadsProcessing;
		
		/// <summary>
		/// The maximum size of <see cref="lightRunnable"/>s (each <see cref="LightRunnable"/> contains a thread).  Must be greater than 0.
		/// </summary>
		size_t maxThreads;
	};
}
