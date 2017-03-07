import Sofa
import sys


class ChainAll(Sofa.PythonScriptController):
	nb_timesteps = 0
	total_time = 0

	# called once the script is loaded
	def onLoaded(self,node):
		nb_timesteps = 0
		total_time = 0
		self.counter = 0
		self.rootNode = node.getRoot()
		print 'Controller script loaded from node %s'%node.findData('name').value
		return 0

	# optionnally, script can create a graph...
	def createGraph(self,node):
		print 'createGraph called (python side)'
		return 0

	# called when necessary by Sofa framework... 
	def cleanup(self):
		print 'empty cleanup called (python side)'
		sys.stdout.flush()
		return 0


	# called on each animation step
	def onBeginAnimationStep(self,dt):
		self.total_time += dt
		self.nb_timesteps += 1
		print 'Beginning of iteration ', self.nb_timesteps 
		print 'dt =', dt 
		print 'total_time =', self.total_time 
		#print 'onBeginAnimatinStep (python) dt=%f total time=%f'%(dt,self.total_time)
		return 0

	def onEndAnimationStep(self,dt):
		if self.total_time+dt >= 1:
			print 'end time reached - End of simulation....'
			self.isAnimated = False
			self.rootNode.getRootContext().animate = False
			sys.exit(0)
			#quit()
		sys.stdout.flush()
		return 0
