## if exists("rstxml-header")
:cpp:{{ kind }}:`{{ rstxml-header }} <{{ rstxml-entity-name }}>`
--------------------------------------------------------------------------------------------------

## endif
.. cpp:{{ kind }}:: {{ decl }}

## if exists("brief")
    {{ brief }}
    
## endif
## if exists("tmpl-params")
    Template Parameters
## for parameter in tmpl-params
        - ``{{ parameter.name }}``: {{ parameter.description }}
## endfor

## endif
## if exists("return")
    Return Type
        {{ return }}
	
## endif
:ref:`{{ file }}` at line {{ file-start }}.
	 
.. literalinclude:: ../../../{{ file }}
   :language: cpp
   :lines: {{ file-start }} - {{ file-end }}
   :linenos:
   :lineno-match:
      
## if exists("detailed")
    {{ detailed }}
    
## endif
## if exists("rstxml-footer")

{{ rstxml-footer }}

## endif
