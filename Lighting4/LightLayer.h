#pragma once
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <queue>
#include <allegro5/bitmap.h>
#include "AboveLightBlocker.h"
#include "LightBlocker.h"

namespace lighting
{
	class LightRunnable;
	class GaussianBlurrer;

	/// <summary>
	/// The core of the lighting system.  Holds all <see cref="LightSource" />s and LightBlockers, handles drawing operations, and manages threads.
	/// </summary>
	class LightLayer
	{
		friend class AboveLightBlocker;
		friend class AboveLightSource;
		friend class CircleLightSource;
		friend class LightSource;
		friend class GaussianBlurrer;

	public:
		static const int MAX_THREAD_TO_CORES = 0;

		/// <summary>
		/// Initializes a new instance of the <see cref="LightLayer"/> class.  
		/// </summary>
		/// <param name="drawToBmpW">The draw to BMP w.</param>
		/// <param name="drawToBmpH">The draw to BMP h.</param>
		/// <param name="lightBmpScale">The light BMP scale.</param>
		/// <param name="maxThreads">The maximum threads. Default will set this to number of cores on computer.</param>
		LightLayer(int drawToBmpW, int drawToBmpH, double lightBmpScale, size_t maxThreads = MAX_THREAD_TO_CORES);
				
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
		/// Accessor for attribute <see cref="lightBmpScale"/>. Scale of <see cref="lightMap"/> relative to the display size.
		/// </summary>
		/// <returns>Scale of <see cref="lightMap"/> relative to the display.</returns>
		float getLightBmpScale()
		{
			return lightBmpScale;
		}
				
		/// <summary>
		/// Gets the number light blockers.
		/// </summary>
		/// <returns></returns>
		int getNumLightBlockers()
		{
			return lightBlockers.size();
		}
		
		/// <summary>
		/// Accessor for data members <see cref="lightMap"/>.
		/// </summary>
		/// <returns>Pointer to <see cref="lightMap"/>.</returns>
		ALLEGRO_BITMAP* getLightMap()
		{
			return lightMap;
		}

		/// <summary>
		/// Finalizes an instance of the <see cref="LightLayer"/>.  None of the <see cref="LightBlocker"/>s or <see cref="LightSource"/> are deleted.  Everything else is destroyed.
		/// </summary>
		~LightLayer();

	private:
		/// <summary>
		/// Allegro bitmap flags for <see cref="lightMap"/>.
		/// </summary>
		static const int LIGHT_MAP_FLAGS = ALLEGRO_NO_PRESERVE_TEXTURE | ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR;

		void addGaussianBlurrer(GaussianBlurrer* blurrer)
		{
			blurrers.push_back(blurrer);
		}

		void removeGaussianBlurrer(GaussianBlurrer* blurrer)
		{
			blurrers.remove(blurrer);
		}
		
		/// <summary>
		/// Adds the <paramref name="aboveLightBlocker"/>.
		/// </summary>
		/// <param name="aboveLightBlocker">The <see cref="AboveLightBLocker"/> to be added.</param>
		void addAboveLightBlocker(AboveLightBlocker* aboveLightBlocker);

		/// <summary>
		/// Removes <paramref name="aboveLightBlocker"/>.
		/// </summary>
		/// <param name="aboveLightBlocker">The <see cref="AboveLightBlocker"/> to be removed.</param>
		void removeAboveLightBlocker(AboveLightBlocker* aboveLightBlocker);

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

		std::list <GaussianBlurrer*> blurrers;

		/// <summary>
		/// The <see cref="LightRunnable"/>s used to handle various <see cref="LightSource"/> methods in a seperate thread.
		/// </summary>
		std::list <LightRunnable*> lightRunnables;

		/// <summary>
		/// Temporarily stores <see cref="LightSource"/> objects when they cannot be directly added.  When <see cref="transferHeldVars()"/> is called, the queue is cleared.
		/// </summary>
		std::queue <LightSource*> heldAddLightSources;
		
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
		/// Stores all of the <see cref="AboveLightBlocker"/>s that will be processed by <see cref="AboveLightSource"/>s.
		/// </summary>
		std::unordered_set <AboveLightBlocker*> aboveLightBlockers;

		/// <summary>
		/// The bitmap where all <see cref="LightSource"/>s are drawn to and blurring and blending operations are preformed.  Initialized by the constructor and is not reassigned.
		/// </summary>
		ALLEGRO_BITMAP* lightMap;
		
		/// <summary>
		/// The bitmap to temporarily store blurs.
		/// </summary>
		ALLEGRO_BITMAP* blurMap;
		
		/// <summary>
		/// The scale of the <see cref="lightMap"/> to the display.
		/// </summary>
		float lightBmpScale;
		
		/// <summary>
		/// Indicates whether the lightRunnables are processing shadows.  Set to <c>true</c> by <see cref="detach()"/ and set to <c>false</c> by <see cref="draw()"/>.
		/// </summary>
		bool threadsProcessing;
		
		/// <summary>
		/// The maximum size of <see cref="lightRunnable"/>s (each <see cref="LightRunnable"/> contains a thread).  Must be greater than 0.
		/// </summary>
		size_t maxThreads;

		/// <summary>
		/// Width of the display the lightMap will be drawn to.
		/// </summary>
		int drawToWidth;

		/// <summary>
		/// Height of the display the lightMap will be drawn to.
		/// </summary>
		int drawToHeight;
	};
}
