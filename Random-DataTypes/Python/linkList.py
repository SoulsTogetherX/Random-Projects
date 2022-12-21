

##======================================================================##
##						LINKED LIST IMPLEMENTATION						##
##======================================================================##




class _linked__node:						#	The Basic Node class
	def __init__(self, data):
		self.data = data
		self.next = None
		self.prev = None

class Linked:								#	The main Data Structure class
	def __init__(self, *args):
		from types import GeneratorType
		self._linked__head = self._linked__tail = None
		self._linked__length = 0

		if not len(args): return None

		for data in args:
			try:
				for val in data:
					self.append(val)
			except TypeError:
				if data is not None: self.append(data)

	def __del__(self):
		self.clear()

	def __repr__(self):
		return '<' + ', '.join(map(lambda data: '<...>' if data is self else "'" + str(data) + "'" if isinstance(data, str) else str(data), self)) + '>'

	def __len__(self):
		return self._linked__length

	def __cmp__(self, other):
		if not isinstance(other, linked): raise TypeError(f'can only compair linked list (not {dict}) to linked list', type(other))
		elif other._linked__length == 0: return self._linked__length > 0
		for selfVal, otherVal in zip(self, other):
			if selfNode != otherVal:
				return (selfVal > otherVal) - (selfVal < otherVal)
		return (self._linked__length > other._linked__length) - (self._linked__length < other._linked__length)

	def __add__(self, other):
		try:
			newList = linked()
			for data in self:
				newList.append(data)
			for data in other:
				newList.append(data)
			return newList
		except TypeError:
			raise TypeError(f'can only concatenate linked lists, lists, sets, or tuples (not {dict}) to linked list', type(other))

	def __iadd__(self, other):
		try:
			for data in other:
				self.append(data)
			return self
		except TypeError:
			raise TypeError(f'can only concatenate linked lists, lists, sets, or tuples (not {dict}) to linked list', type(other))

	def __mul__(self, mul):
		try:
			newList = linked()
			for _ in range(mul):
				for data in self:
					newList.append(data)
			return newList
		except TypeError:
			print(f"TypeError: can't multiply sequence by non-int of type '{type(mul)}'")

	def __imul__(self, mul):
		try:
			length = self._linked__length
			for _ in range(mul-1):
				for data in self[:length]:
					self.append(data)
			return self
		except TypeError:
			print(f"TypeError: can't multiply sequence by non-int of type '{type(mul)}'")
			return self

	def __nonzero__(self):
		return self._linked__length > 0

	def __setitem__(self, indx, data):
		if isinstance(indx, int):
			next(self._linked__getnodes(indx)).data  = data
		elif isinstance(indx, slice) or indx is None:
			for currNode in self._linked__getnodes(indx):
				currNode.data = data
		else:
			raise TypeError(f'linked list indices must be integers or slices, not {type(indx)}')

	def __getitem__(self, indx):
		if isinstance(indx, int):
			return next(self._linked__getnodes(indx)).data
		elif isinstance(indx, slice):
			start, stop, step = indx.indices(len(self))
			newList = linked()
			if indx.start <= self._linked__length - indx.stop:
				for currNode in self._linked__getnodes(indx):
					newList.append(currNode.data)
			else:
				for currNode in self._linked__getnodes(indx):
					newList.push(currNode.data)
			return newList
		else:
			raise TypeError(f'linked list indices must be integers or slices, not {type(indx)}')

	def __iter__(self):
		try:
			currNode = self._linked__head
		except:
			return None
		while currNode != None:
			yield currNode.data
			currNode = currNode.next

	def __reversed__(self):
		try:
			currNode = self._linked__tail
		except:
			return None
		while currNode != None:
			yield currNode.data
			currNode = currNode.prev

	def __contains__(self, item):
		for currNode in self:
			if currNode == data:
				return True
		return False

	def __sizeof__(self):
		import sys
		size = 0
		for currNode in self._linked__getnodes():
			size+=sys.getsizeof(currNode)
		return sys.getsizeof((self._linked__head)) + sys.getsizeof((self._linked__tail)) + sys.getsizeof(self._linked__length) + size

	def _linked__getnodes(self, indx = slice(None,None,1)):
		if isinstance(indx, int):
			if not (-self._linked__length <= indx < self._linked__length): raise IndexError("link list index out of range")
			elif indx < 0: indx = self._linked__length+indx
			if ((self._linked__length/2)-indx) >= 0:
				currNode = self._linked__head
				for _ in range(indx):
					currNode = currNode.next
				yield currNode
			else:
				currNode = self._linked__tail
				for _ in range(self._linked__length-indx-1):
					currNode = currNode.prev
				yield currNode
		elif isinstance(indx, slice):
			start, stop, step = indx.indices(len(self))
			if start <= self._linked__length - stop:
				currNode = self._linked__head
				for _ in range(start):
					currNode = currNode.next
				stop -= start; start = 0
				while currNode != None and start < stop:
					if not start%step: yield currNode
					start += 1
					currNode = currNode.next
			else:
				currNode = self._linked__tail
				for _ in range(self._linked__length - stop):
					currNode = currNode.prev
				stop -= start; start = 0
				while currNode != None and start < stop:
					if not stop%step: yield currNode
					stop -= 1
					currNode = currNode.prev
		else:
			raise TypeError(f'linked list indices must be integers or slices, not {type(indx)}')

	def push(self, data):
		"""
		Places a value at the start of the linked list
		"""
		try:
			self._linked__head.prev = _linked__node(data)
			self._linked__head.prev.next = self._linked__head
			self._linked__head = self._linked__head.prev
		except:
			self._linked__head = self._linked__tail = _linked__node(data)
		self._linked__length += 1

	def pop(self):
		"""
		Removes a value at the start of the linked list and returns the data value that was in it
		"""
		if self._linked__length == 0:
			raise IndexError("pop from empty linked list")
		data = self._linked__head.data
		self._linked__head = self._linked__head.next
		try:
			self._linked__head.prev = None
		except:
			pass
		self._linked__length -= 1 
		return data

	def append(self, data):
		"""
		Places a value at the end of the linked list
		"""
		try:
			self._linked__tail.next = _linked__node(data)
			self._linked__tail.next.prev = self._linked__tail
			self._linked__tail = self._linked__tail.next
		except:
			self._linked__head = self._linked__tail = _linked__node(data)
		self._linked__length += 1

	def remove(self):
		"""
		Removes a value at the end of the linked list and returns the data value that was in it
		"""
		if self._linked__length == 0:
			raise IndexError("remove from empty linked list")
		data = self._linked__tail.data
		self._linked__tail = self._linked__tail.prev
		try:
			self._linked__tail.next = None
		except:
			pass
		self._linked__length -= 1
		return data

	def head(self):
		"""
		Returns the value at the head of the linked list
		"""
		if self._linked__length == 0:
			raise IndexError("attempting to access data from an empty linked list")
		return self._linked__head.data

	def tail(self):
		"""
		Returns the value at the tail of the linked list
		"""
		if self._linked__length == 0:
			raise IndexError("attempting to access data from an empty linked list")
		return self._linked__tail.data

	def findIdx(self, data):
		"""
		Attempts to find all the indexes of the given data in the linked list
		"""
		idx = 0
		for currNode in self:
			if currNode == data:
				yield idx
			idx += 1

	def sort(self, headIndx = 0, tailIndex = -1):
		"""
		Uses the MergeSort algorithm to quickly sort a section of the link list from smallest to greatest
		"""
		if (headIndx + len(self))%len(self) > (tailIndex + len(self))%len(self): return None
		head, tail = next(self._linked__getnodes(headIndx)), next(self._linked__getnodes(tailIndex))
		before, after = head.prev, tail.next
		head, tail = self._linked__mergesort(head, tail)

		if not before: self._linked__head = head
		else: head.prev = before; before.next = head

		if not after: self._linked__tail = tail
		else: tail.next = after; after.prev = tail

	def _linked__mergesort(self, head, tail):
		if head is None or head.next is None: return (head, tail)

		mid, tracer = head, head.next
		while tracer and tracer is not tail:
			tracer = tracer.next
			if tracer and tracer is not tail:
				mid = mid.next
				tracer = tracer.next
		marker1, marker2 = head, mid.next
		mid.next, tail.next = None, None

		marker1, _ = self._linked__mergesort(marker1, mid)
		marker2, _ = self._linked__mergesort(marker2, tail)

		if marker1.data <= marker2.data:
			head = tail = marker1
			marker1 = marker1.next
		else:
			head = tail = marker2
			marker2 = marker2.next
		tail.prev = None

		while marker1 or marker2:
			if not marker2 or marker1 and marker1.data <= marker2.data:
				marker1.prev = tail
				tail.next = tail = marker1
				marker1 = marker1.next
			elif marker2:
				marker2.prev = tail
				tail.next = tail = marker2
				marker2 = marker2.next
		return (head, tail)

	def sortReversed(self, headIndx = 0, tailIndex = -1):
		"""
		Uses the MergeSort algorithm to quickly sort the link list from greatest to smallest 
		"""
		if (headIndx + len(self))%len(self) > (tailIndex + len(self))%len(self): return None
		head, tail = next(self._linked__getnodes(headIndx)), next(self._linked__getnodes(tailIndex))
		before, after = head.prev, tail.next
		head, tail = self._linked__mergesortReversed(head, tail)

		if not before: self._linked__head = head
		else: head.prev = before; before.next = head

		if not after: self._linked__tail = tail
		else: tail.next = after; after.prev = tail

	def _linked__mergesortReversed(self, head, tail):
		if head is None or head.next is None: return (head, tail)

		mid, tracer = head, head.next
		while tracer and tracer is not tail:
			tracer = tracer.next
			if tracer and tracer is not tail:
				mid = mid.next
				tracer = tracer.next
		marker1, marker2 = head, mid.next
		mid.next, tail.next = None, None

		marker1, _ = self._linked__mergesortReversed(marker1, mid)
		marker2, _ = self._linked__mergesortReversed(marker2, tail)

		if marker1.data >= marker2.data:
			head = tail = marker1
			marker1 = marker1.next
		else:
			head = tail = marker2
			marker2 = marker2.next
		tail.prev = None

		while marker1 or marker2:
			if not marker2 or marker1 and marker1.data >= marker2.data:
				marker1.prev = tail
				tail.next = tail = marker1
				marker1 = marker1.next
			elif marker2:
				marker2.prev = tail
				tail.next = tail = marker2
				marker2 = marker2.next
		return (head, tail)

	def replace(self, searchVal, data):
		"""
		Replaces all instances of the first argument with the second argument in the linked list
		"""
		for currNode in self._linked__getnodes():
			if currNode.data == searchVal: currNode.data = data

	def funcIndx(self, indx, func, argv = []):
		"""
		Replaces the value at the given index in the linked list with the result of a function (first argument) with the
		current index's data and the values in the given array (second argument) as arguments
		"""
		currNode = self._linked__getnodes(indx)
		currNode.data = func(currNode.data, *argv)

	def funcAll(self, indx, func, argv = []):
		"""
		Replaces the value at all indexes in the linked list with the result of a function (first argument) with the
		current index's data and the values in the given array (second argument) as arguments
		"""
		currNode = self._linked__head
		while currNode != None:
			currNode.data = func(currNode.data, *argv)
			currNode = currNode.next

	def clear(self):
		"""
		Removes all items in the linked list
		"""
		for currNode in self._linked__getnodes():
			del currNode
		self._linked__head = self._linked__tail = None
		self._linked__length = 0

def linked(data):
	"""
	Turns the given input into a linked list
	"""
	try:
		iter(data)
	except TypeError:
		raise TypeError(f"'{type(data)}' object is not iterable")
	else:
		LL = Linked(data)
	return LL